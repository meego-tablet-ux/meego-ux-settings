#include <QDBusPendingCallWatcher>
#include <QDBusServiceWatcher>

#include <syncevolution-qt-dbus/syncevo-server-full.h>

#include "SyncEvoStorageModel.h"
#include "SyncEvoStatic.h"

/*
 * Construct a storage model:
 * Attach() to the D-Bus interface, and look out for the backend's death.
 */
MeeGo::Sync::SyncEvoStorageModel::SyncEvoStorageModel(QObject *parent)
  : QAbstractListModel(parent)
  , m_watcher(0)
  , m_serverInterface(new OrgSyncevolutionServerInterface("org.syncevolution", "/org/syncevolution/Server", QDBusConnection::sessionBus(), this))
  , m_error(false)
  , m_serverDBusName()
{
  SyncEvoStatic::init();
  QDBusPendingReply<> reply = m_serverInterface->Attach();
  reply.waitForFinished();

  if (reply.isError()) {
    m_error = true;
    SyncEvoStatic::reportDBusError(__PRETTY_FUNCTION__ + QString(": Attach() failed: "), reply.error());
  }
  else {
    m_serverDBusName = reply.reply().service();
    connect(
      new QDBusServiceWatcher(
        m_serverDBusName,
        QDBusConnection::sessionBus(),
        QDBusServiceWatcher::WatchForOwnerChange,
        this),
      SIGNAL(serviceOwnerChanged(const QString &, const QString &, const QString &)),
      this,
      SLOT(serviceOwnerChanged(const QString &, const QString &, const QString &)));

    QHash<int, QByteArray> roles; 
    roles[DisplayNameRole] = "displayName";
    roles[NameRole]        = "name"; // "id" is an existing QML property.
    roles[ImageRole]       = "image";
    roles[ActiveRole]      = "active";
    roles[StorageRole]     = "storage";
    setRoleNames(roles);

    /*
     * Retrieve the configurations
     */
    SyncEvoStatic::dbusCall(
      QList<QProperty>()
        << QProperty("DBusFunctionName", "GetConfigs")
        << QProperty("Template", true),
      this, SLOT(asyncCallFinished(QDBusPendingCallWatcher *)),
      m_serverInterface->GetConfigs(true));
  }
}

/*
 * Detach when dying.
 */
MeeGo::Sync::SyncEvoStorageModel::~SyncEvoStorageModel()
{
  if (!m_error)
    m_serverInterface->Detach().waitForFinished();
}

/*
 * How many rows does this model have?
 */
int
MeeGo::Sync::SyncEvoStorageModel::rowCount(QModelIndex const & /* parent */) const
{
  return m_error ? 0 : m_l.count();
}

/*
 * Retrieve a single piece of data.
 */
QVariant
MeeGo::Sync::SyncEvoStorageModel::data(QModelIndex const & index, int role) const
{
  int row = index.row();
  QVariant retVal = QVariant();

  if (!(m_error || row < 0 || row >= m_l.count())) {
    switch (role) {
      case DisplayNameRole:
        retVal = m_l[row].displayName();
        break;

      case NameRole:
        retVal = m_l[row].name();
        break;

      case ImageRole:
        retVal = m_l[row].iconURI();
        break;

      case ActiveRole:
        retVal = false;
        break;

      case StorageRole:
        retVal = m_l[row].storage();
        break;

      default:
        break;
    }
  }

  return retVal;
}

/*
 * Handle asynchronous D-Bus calls. Since there are only two for this class, we don't need to turn this function into a
 * dispatcher.
 *
 * GetConfigs returns a list of the available configs. For each item in the list, we place a GetConfig call to retrieve
 * the template configuration for that particular config name.
 *
 * GetConfig returns the template configuration for a given service. We may or may not add the configuration to the list.
 */
void
MeeGo::Sync::SyncEvoStorageModel::asyncCallFinished(QDBusPendingCallWatcher *call)
{
  if (!m_error) {

    if ("GetConfigs" == DBUS_CALL_FUNCTION_NAME(call)) {
      QDBusPendingReply<QStringList> reply = *call;
      QStringList configs = reply.argumentAt<0>();
      if (reply.isError()) {
        SyncEvoStatic::reportDBusError(QString(__PRETTY_FUNCTION__), reply.error());
        m_error = true;
      }
      else {
        for (int Nix = 0 ; Nix < configs.count() ; Nix++)
          SyncEvoStatic::dbusCall(
            QList<QProperty>()
              << QProperty("DBusFunctionName", "GetConfig")
              << QProperty("Template", call->property("Template").toBool())
              << QProperty("ConfigName", reply.argumentAt<0>()[Nix]),
            this, SLOT(asyncCallFinished(QDBusPendingCallWatcher *)),
            m_serverInterface->GetConfig(
              reply.argumentAt<0>()[Nix] + 
                (call->property("Template").toBool()
                  ? ("@" + QUuid::createUuid().toString())
                  : QString("")),
              call->property("Template").toBool()));

        /*
         * Retrieve the actual configurations as well
         */
        if (call->property("Template").toBool())
          SyncEvoStatic::dbusCall(
            QList<QProperty>()
              << QProperty("DBusFunctionName", "GetConfigs")
              << QProperty("Template", false),
            this, SLOT(asyncCallFinished(QDBusPendingCallWatcher *)),
            m_serverInterface->GetConfigs(false));
      }
    }

    else
    if ("GetConfig" == DBUS_CALL_FUNCTION_NAME(call)) {
      QDBusPendingReply<QStringMultiMap> reply = *call;
      if (reply.isError()) {
        SyncEvoStatic::reportDBusError(QString(__PRETTY_FUNCTION__), reply.error());
        if (reply.error().name() != "org.syncevolution.Exception")
          m_error = true;
      }
      else
        maybeAddToList(call->property("ConfigName").toString(), reply.argumentAt<0>(), call->property("Template").toBool());
    }
  }
  call->deleteLater();
}

/*
 * Process a single config:
 * 1. Decide whether this config is to be processed at all.
 * 2. If yes, create one row for each recognized source in this config.
 *    A source, such as "source/calendar" is "recognized", if we can give it a friendly name.
 *
 *    Friendly names are currently defined in the static hash table SyncEvoStatic::storageTypes().
 *
 *    The header, which is the "source" whose name is "", is copied to each row.
 *
 *    The rows are grouped by storage type. A separate array, m_storageTypeIndices, keeps track
 *    of where in the main array a given storage type ends, so that when a row of that type is
 *    to be added, it is added there, and the m_storageTypeIndices entry for that storage type,
 *    as well as the entries for each subsequent storage type are incremented.
 *
 * FIXME: Could this be done using references, to avoid all the copying/duplication?
 */
void
MeeGo::Sync::SyncEvoStorageModel::maybeAddToList(const QString &configName, const QStringMultiMap &config, bool isTemplate)
{
  /* Not interested if it's a local config for some other config */
  if (configName.startsWith("source-config@"))
    return;

  /* Not interested if there's no header */
  if (!config.contains(""))
    return;

  /* Not interested if this is a template for a device */
  if (config[""].contains("PeerIsClient"))
    return;

  /* Not interested if this is not "ConsumerReady" */
  if (!config[""].contains("ConsumerReady") || config[""]["ConsumerReady"] != "1")
      return;

  /* For each source associated with this config ... */
  QMapIterator<QString, QStringMap> itr(config);
  while(itr.hasNext()) {
    itr.next();

    /* If this type of source is recognized ... */
    if (SyncEvoStatic::storageTypes().contains(itr.key())) {

      /* If the source has "sync" defined ... */
      if (itr.value().contains("sync")) {
        SyncEvoStorageModelItem newItem = SyncEvoStorageModelItem(configName, itr.key(), config[""], itr.value(), isTemplate);

        bool replace = false;
        int storageTypeIdx = -1;
        int idxInsert = getInsertIndex(newItem, &replace, &storageTypeIdx);

        if (idxInsert >= 0) {
          if (replace) {
            m_l[idxInsert] = newItem;
            emit dataChanged(index(idxInsert, 0), index(idxInsert, 0));
          }
          else {
            /* Insert the value */
            beginInsertRows(QModelIndex(), idxInsert, idxInsert);
            m_l.insert(idxInsert, newItem);
            /* ... and increment the index indicating where the current and all the subsequent storage types end */
            for (int Nix = storageTypeIdx ; Nix < m_storageTypeIndices.count() ; Nix++)
              m_storageTypeIndices[Nix].set(m_storageTypeIndices[Nix].value().toInt() + 1);
            endInsertRows();
          }
        }
      }
    }
  }
}

int
MeeGo::Sync::SyncEvoStorageModel::getInsertIndex(const SyncEvoStorageModelItem &newItem, bool *replace, int *storageTypeIdx)
{
  /* By default, append */
  int idxInsert = m_l.count();
  *replace = false;
  *storageTypeIdx = m_storageTypeIndices.count();

  /* If we've added this type of source before ... */
  if (m_storageTypeIndices.contains(QProperty(newItem.storage()))) {
    *storageTypeIdx = m_storageTypeIndices.indexOf(newItem.storage());
    /* insert this into the array such that it goes to one-before the end of the list of same-source entries */
    idxInsert = m_storageTypeIndices[*storageTypeIdx].value().toInt();
    /*
     * ... but not before checking whether it can replace an existing array member,
     * by checking whether it can replace any entry of identical name in the portion of the array
     * holding members of that storage type
     */
    int idxBeg =
      (0 == (*storageTypeIdx)
        ? 0
        : m_storageTypeIndices[(*storageTypeIdx) - 1].value().toInt() + 1);

    for (int Nix = idxBeg ; Nix <= idxInsert ; Nix++)
      /*
       * No point checking the storage here, because we only compare values of the same storage type, however,
       * the two items have to have the same storage type for us to consider replacement/dropping.
       *
       * The name comparison is done in a case-insensitive manner because of BMC#18864
       */
      if (0 == newItem.name().compare(m_l[Nix].name(), Qt::CaseInsensitive)
          /*&& newItem.storage() == m_l[Nix].storage()*/) {
        if (m_l[Nix].isTemplate() && !newItem.isTemplate()) {
          /*
           * Indicate that m_l[Nix] should be replaced by newItem;
           */
          idxInsert = Nix;
          *replace = true;
          break;
        }
        else
        if (!m_l[Nix].isTemplate() && newItem.isTemplate())
          /*
           * Indicate that the new item should be discarded
           */
          idxInsert = -1;
      }
  }
  else {
    /* Never-before-encountered source type: append to the end and create new source type index entry */
    m_storageTypeIndices << QProperty(newItem.storage(), m_l.count());
    *storageTypeIdx = m_storageTypeIndices.count();
    idxInsert = m_l.count();
  }

  return idxInsert;
}

/*
 * If the unique number for the syncevo D-Bus server changes, it must be because it has died. So, curl up and die, too.
 */
void
MeeGo::Sync::SyncEvoStorageModel::serviceOwnerChanged(const QString &name, const QString &oldOwner, const QString &newOwner)
{
  Q_UNUSED(name)
  Q_UNUSED(oldOwner)
  Q_UNUSED(newOwner)

  if (name == m_serverDBusName) {
    emit serviceHasDied();
    m_error = true;
  }
}
