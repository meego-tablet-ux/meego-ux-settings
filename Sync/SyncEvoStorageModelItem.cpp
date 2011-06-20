#include "SyncEvoStatic.h"
#include "SyncEvoStorageModelItem.h"

MeeGo::Sync::SyncEvoStorageModelItem::SyncEvoStorageModelItem(const QString &name, const QString &storage, const QStringMap &header, const QStringMap &storageInfo, bool isTemplate)
      : m_name(name)
      , m_storage(storage)
      , m_header(header)
      , m_storageInfo(storageInfo)
      , m_isTemplate(isTemplate)
{
}

QString
MeeGo::Sync::SyncEvoStorageModelItem::displayName() const
{
  QString ret =
    (m_header.contains("PeerName")
      ? m_header["PeerName"]
      : m_name);
  /*
   * FIXME: Ugly hack for Google
   */
  return (ret.toLower().startsWith("google"))
    ? "Google"
    : ret;
}

QString
MeeGo::Sync::SyncEvoStorageModelItem::name() const
{
  return m_name + QString(QChar('\0')) + m_storage;
}

QString
MeeGo::Sync::SyncEvoStorageModelItem::storage() const
{
  return SyncEvoStatic::storageTypes()[m_storage];
}

QStringMap
MeeGo::Sync::SyncEvoStorageModelItem::header() const
{
  return m_header;
}

QStringMap
MeeGo::Sync::SyncEvoStorageModelItem::storageInfo() const
{
  return m_storageInfo;
}

QString
MeeGo::Sync::SyncEvoStorageModelItem::toString() const
{
  return QString() + "name: \"" + m_name + "\" storage: \"" + m_storage + "\" template: " + (m_isTemplate ? "true" : "false"); 
}

bool
MeeGo::Sync::SyncEvoStorageModelItem::isTemplate() const
{
  return m_isTemplate;
}
