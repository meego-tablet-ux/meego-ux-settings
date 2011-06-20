/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include <syncevolution-qt-dbus/syncevo-server-full.h>
#include <syncevolution-qt-dbus/syncevo-session-full.h>

#include <QDBusConnection>
#include <QDebug>

#include "SyncEvoFrameworkClient.h"
#include "SyncEvoStatic.h"

#define IS_LOCAL_CONFIG(c) \
  (((c).contains("")) && \
   ((c)[""].contains("syncURL")) && \
   ((c)[""]["syncURL"].startsWith("local://")))

MeeGo::Sync::SyncEvoFrameworkClient::SyncEvoFrameworkClient(QObject* parent)
  : QObject(parent)
  , m_serverInterface(new OrgSyncevolutionServerInterface("org.syncevolution", "/org/syncevolution/Server", QDBusConnection::sessionBus(), this))
  , m_sessionInterface(0)
  , m_inProgress(false)
  , m_error(false)
  , m_scheduled(false)
  , m_status()
  , m_service()
  , m_storage()
  , m_name()
  , m_username()
  , m_password()
  , m_serverDBusName()
{
  /*
   * Attach to server on creation
   */
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
  }
}

MeeGo::Sync::SyncEvoFrameworkClient::~SyncEvoFrameworkClient()
{
  /*
   * Detach from server on destruction
   */
  if (!m_error)
    m_serverInterface->Detach().waitForFinished();
}

bool
MeeGo::Sync::SyncEvoFrameworkClient::scheduled() const
{
  return m_scheduled;
}

void
MeeGo::Sync::SyncEvoFrameworkClient::setScheduled(bool s)
{
  // Be careful not to emit the scheduledChanged() signal if the
  // "scheduled" value has not changed.

  if (s != m_scheduled) {
    m_scheduled = s;
    m_config[""]["autoSync"] = s ? "1" : "0";
    emit scheduledChanged(s);
  }
}

QString
MeeGo::Sync::SyncEvoFrameworkClient::status() const
{
  return m_status;
}

void
MeeGo::Sync::SyncEvoFrameworkClient::setStatus(QString s)
{
  // Be careful not to emit the statusChanged() signal if the value of
  // the status string has not changed.

  if (s != m_status) {
    m_status = s;
    emit statusChanged(s);
  }
}

QString
MeeGo::Sync::SyncEvoFrameworkClient::service() const
{
  return m_service;
}

void
MeeGo::Sync::SyncEvoFrameworkClient::setService(QString s)
{
  if (s != m_service)
    m_service = s;
}

QString
MeeGo::Sync::SyncEvoFrameworkClient::storage() const
{
  return m_storage;
}

void
MeeGo::Sync::SyncEvoFrameworkClient::setStorage(QString s)
{
  if (s != m_storage) {
    m_storage = s;
  }
}

QString
MeeGo::Sync::SyncEvoFrameworkClient::name() const
{
  return m_name;
}

void
MeeGo::Sync::SyncEvoFrameworkClient::setName(QString s)
{
  if (s != m_name) {
    m_name = s;

    /*
     * Once we know the service name (== the config in syncevo lingo) we can get the detais and the last known status
     */
    if (!m_error) {
      SyncEvoStatic::dbusCall(
        QList<QProperty>()
          << QProperty("DBusFunctionName", "GetConfig")
          << QProperty("ConfigName", s),
        this, SLOT(asyncCallFinished(QDBusPendingCallWatcher *)),
        m_serverInterface->GetConfig(s, false));

      /*
       * Has to be done sync so the report is available for when lastSyncTime() is called by the UI - otherwise
       * we don't have a fuzzyTime :(
       */
      SyncEvoStatic::dbusCall(
        QList<QProperty>()
          << QProperty("DBusFunctionName", "GetReports")
          << QProperty("ConfigName", s),
        this, SLOT(asyncCallFinished(QDBusPendingCallWatcher *)),
        m_serverInterface->GetReports(s, 0, 1))->waitForFinished();
    }
  }
}

QString
MeeGo::Sync::SyncEvoFrameworkClient::username() const
{
  return m_username;
}

/*
 * Changes to the username and password do not cause a SetConfig to be issued (i.e., the configuration is not
 * saved immediately), because the current UX is such that once there's a working username/password, it is kept
 * until the config is explicitly destroyed by the user. Conversely, if there's no associated username/password,
 * the user is prompted, and, once entered, a sync is performed immediately. A sync is defined to require a SetConfig,
 * so the username/password is in effect saved as soon as the config is created.
 */
void
MeeGo::Sync::SyncEvoFrameworkClient::setUsername(QString s)
{
  if (s != m_username) {
    m_username = s;
    /* Don't save the username to a local config */
    if (!IS_LOCAL_CONFIG(m_config))
      m_config[""]["username"] = s;
    emit usernameChanged(s);
  }
}

QString
MeeGo::Sync::SyncEvoFrameworkClient::password() const
{
  return m_password;
}

void
MeeGo::Sync::SyncEvoFrameworkClient::setPassword(QString s)
{
  if (s != m_password) {
    m_password = s;
    /* Don't save the password to a local config */
    if (!IS_LOCAL_CONFIG(m_config))
      m_config[""]["password"] = s;
    emit passwordChanged(s);
  }
}

QDateTime
MeeGo::Sync::SyncEvoFrameworkClient::lastSyncTime()
{
  QDateTime ret = QDateTime();

  if (m_lastReport.contains("end"))
    ret = QDateTime::fromMSecsSinceEpoch(((qint64)m_lastReport["end"].toUInt()) * 1000);

  return ret;
}

void
MeeGo::Sync::SyncEvoFrameworkClient::doPostInit(QString fuzzyTime,
					 bool forceSync)
{
  if (forceSync)
    doInitialSync();  // Never been "synced".
  else
    setStatusFromLastReport(fuzzyTime);
}

void
MeeGo::Sync::SyncEvoFrameworkClient::doInitialSync()
{
  qDebug() << "INFO: Performing 'initial sync' procedure for profile:"
	   << m_name;

  if (m_username.isEmpty())
    // No user name set yet.  Query the user for the username and
    // password by popping up the login dialog.
    emit authenticationFailed();
  else
    syncNow();
}

void
MeeGo::Sync::SyncEvoFrameworkClient::syncNow()
{
  performAction(Sync);
}

void
MeeGo::Sync::SyncEvoFrameworkClient::enableAutoSync(bool enable)
{
  setScheduled(enable);
  performAction(RememberAutoSync);
}

/*
 * The status is calculated based on the value of m_lastReport. The default is "Unknown sync status".
 * 200 (OK) is treated specially (success). So is 20017 (aborted), although currently the sync cannot be
 * aborted from the UI except if the config is removed while the sync is in progress.
 */
void
MeeGo::Sync::SyncEvoFrameworkClient::setStatusFromLastReport(const QString &fuzzyTime)
{
  QString statusMessage = tr("Unknown sync status");

  if (m_lastReport.contains("status")) {
    int status = m_lastReport["status"].toUInt();

    if (200 == status)
      statusMessage = tr("Last sync %1").arg(
        fuzzyTime.isEmpty()
          ? lastSyncTime().toString("yyyy-MM-dd hh:mm:ss.zzz")
          : fuzzyTime);
    else {
      QString failure =
        SyncEvoStatic::httpStatusCodes().contains(status)
          ? SyncEvoStatic::httpStatusCodes()[status]
          : 20017 == status
            ? tr("Sync aborted")
            : m_lastReport.contains("error")
              ? m_lastReport["error"]
              : m_lastReport["status"];

      statusMessage = tr("Last sync failed: %1").arg(failure);
    }
  }

  setStatus(statusMessage);
}

/*
 * Upon Abort(), we simply check for a D-Bus error - nothing more.
 */
void
MeeGo::Sync::SyncEvoFrameworkClient::handleAbort(QDBusPendingCallWatcher *call)
{
  QDBusPendingReply<> reply = *call;

  if (reply.isError()) {
    SyncEvoStatic::reportDBusError(QString(__PRETTY_FUNCTION__), reply.error());
    m_error = true;
  }
}

/*
 * Upon GetConfig, we try to retrieve the template configuration, if the non-template configuration is absent.
 * Once retrieved, we set the name, username, password, and scheduled properties. If username and/or password are
 * absent, we emit the authenticationFailed() signal, which causes a login dialog to be popped, giving us the
 * opportunity to acquire a username/password from the user.
 */
void
MeeGo::Sync::SyncEvoFrameworkClient::handleGetConfig(QDBusPendingCallWatcher *call)
{
  QDBusPendingReply<QStringMultiMap> reply = *call;
  bool haveLoginInfo = false;

  if (reply.isError()) {
    /* An error retrieving the config ... */
    SyncEvoStatic::reportDBusError(QString(__PRETTY_FUNCTION__), reply.error());

    if (!((call->property("TriedTemplate").isValid() && call->property("TriedTemplate").toBool()) || m_error)) {
      /* If we haven't tried retrieving the template config yet, let's do that first */
      SyncEvoStatic::dbusCall(
        QList<QProperty>()
          << QProperty("DBusFunctionName", "GetConfig")
          << QProperty("ConfigName", m_name)
          << QProperty("TriedTemplate", true),
        this, SLOT(asyncCallFinished(QDBusPendingCallWatcher *)),
        m_serverInterface->GetConfig(m_name, true));
    }
    else {
      /* We should never get here (TM) */
      emit profileRemoved(m_name);
      m_error = true;
    }
  }
  else {
    QStringMultiMap theConfig = reply.argumentAt<0>();

    if (!(call->property("WebDAVConfig").isValid() && call->property("WebDAVConfig").toBool()))
      m_config = reply.argumentAt<0>();

    /* Check if this is a local config and, if so, retrieve the source config */
    if (IS_LOCAL_CONFIG(theConfig)) {

      QString sourceConfig =
        "source-config" + 
        theConfig[""]["syncURL"].right(theConfig[""]["syncURL"].length() - 8/*strlen("local://")*/);

      SyncEvoStatic::dbusCall(
        QList<QProperty>()
          << QProperty("DBusFunctionName", "GetConfig")
          << QProperty("WebDAVConfig", true)
          << QProperty("ConfigName", m_name),
        this, SLOT(asyncCallFinished(QDBusPendingCallWatcher *)),
        m_serverInterface->GetConfig(sourceConfig, false));
    }
    else 
    if (!sessionActions.isEmpty() && sessionActions.head() == SaveWebDAVLoginInfo) {
      theConfig[""]["username"] = m_username;
      theConfig[""]["password"] = m_password;
      SyncEvoStatic::dbusCall(
        QList<QProperty>()
          << QProperty("DBusFunctionName", "SetConfig")
          << QProperty("ConfigName", m_name),
        this, SLOT(asyncCallFinished(QDBusPendingCallWatcher *)),
        m_sessionInterface->SetConfig(false, false, theConfig));
    }
    else {
      /* Grab the interesting data from the retrieved config */
      if (theConfig.contains("")) {
        if (theConfig[""].contains("username")) {
          haveLoginInfo = true;
          setUsername(theConfig[""]["username"]);
        }
        if (theConfig[""].contains("password"))
          setPassword(theConfig[""]["password"]);
        else
          haveLoginInfo = false;

        setScheduled(
          theConfig[""].contains("autoSync")
            ? "1" == theConfig[""]["autoSync"]
              ? true
              : false
            : false);
      }

      /* If the config had no username/password, prompt for it */
      if (!haveLoginInfo)
        emit authenticationFailed();
    }
  }
}

/*
 * Upon GetReports, we attempt to retrieve the last report. If we fail, we attempt to construct a rudimentary
 * report containing only a "status" field from the "Error" property of the call watcher.
 * This is the place to handle authentication failures (401 and 403).
 */
void
MeeGo::Sync::SyncEvoFrameworkClient::handleGetReports(QDBusPendingCallWatcher *call)
{
  QDBusPendingReply<QArrayOfStringMap> reply = *call;

  /* Clear the last report */
  m_lastReport.clear();

  if (reply.isError())
    SyncEvoStatic::reportDBusError(QString(__PRETTY_FUNCTION__), reply.error());
  else {
    QArrayOfStringMap reports = reply.argumentAt<0>();

    if (reports.count() > 0)
      m_lastReport = reports[0];
  }

  /* Even if the report retrieval fails, the PendingCallWatcher may have an "Error" field.
   * If so, and the report doesn't have a status field, create one from the "Error" field. */
  if (!m_lastReport.contains("status") && call->property("Error").isValid())
    m_lastReport["status"] = call->property("Error").toString();

  setStatusFromLastReport();

  /* Give the user a chance to re-enter username/password in case there was a 401 or a 403 */
  if (m_lastReport.contains("status")) {
    unsigned int errorCode = m_lastReport["status"].toUInt();

    if (errorCode >= 10000 && errorCode <= 10599)
      errorCode -= 10000;

    if (401 == errorCode || 403 == errorCode)
      emit authenticationFailed();
  }
}

/*
 * SetConfig is called at the start of any session-based activity. There are currently 3 session-based tasks:
 * 1. When forgetting a profile, the config is set to blank (QStringMultiMap()).
 * 2. When a sync is performed, SetConfig is called first, to save the possible username/password update.
 * 3. When autoSync is toggled, the autoSync field must be saved.
 * The call watcher's "ErrorMessage" property is used in case of a D-Bus error. It depends on which of the above three
 * reasons has caused this call and may be set when making this call.
 */
void
MeeGo::Sync::SyncEvoFrameworkClient::handleSetConfig(QDBusPendingCallWatcher *call)
{
  QDBusPendingReply<> reply = *call;

  if (reply.isError()) {
    SyncEvoStatic::reportDBusError(QString(__PRETTY_FUNCTION__), reply.error());
    m_error = true;

    switch (sessionActions.head()) {
      case Forget:
        setStatus(tr("Unable to forget sync account!"));
        break;

      case RememberAutoSync:
        setStatus(tr("Sync aborted"));
        break;

      default:
        break;
    }
  }
  else {
    bool performDetach = false;
    QList<QProperty> detachProps;
    detachProps
      << QProperty("DBusFunctionName", "Session::Detach")
      << QProperty("ConfigName", m_name);

    switch(sessionActions.head()) {
      case Forget:
        if (IS_LOCAL_CONFIG(m_config)) {
          m_config[""].remove("syncURL");
          detachProps << QProperty("WebDAVConfig", true);
        }
        else
          emit profileRemoved(m_name);
        performDetach = true;
        break;

      case RememberAutoSync:
        /* The session has fulfilled its purpose. Detach. */
        performDetach = true;
        break;

      case Sync:
        if (m_config[""].contains("peerType") && "WebDAV" == m_config[""]["peerType"]) {
          detachProps << QProperty("WebDAVConfig", true);
          performDetach = true;
        }
        else {
          if (!m_error)
            SyncEvoStatic::dbusCall(
              QList<QProperty>()
                << QProperty("DBusFunctionName", "Sync")
                << QProperty("ConfigName", m_name),
              this, SLOT(asyncCallFinished(QDBusPendingCallWatcher *)),
              m_sessionInterface->Sync(QString(), m_config[SyncEvoStatic::reverseStorageTypes()[m_storage]]));
        }
        break;

      case SaveWebDAVLoginInfo:
        performDetach = true;
        break;
    }

    if (performDetach && !m_error)
      SyncEvoStatic::dbusCall(
        detachProps,
        this, SLOT(asyncCallFinished(QDBusPendingCallWatcher *)),
        m_sessionInterface->Detach());
  }
}

/*
 * Upon Sync we simply check if there was a D-Bus error
 */
void
MeeGo::Sync::SyncEvoFrameworkClient::handleSync(QDBusPendingCallWatcher *call)
{
  QDBusPendingReply<QDBusObjectPath> reply = *call;

  if (reply.isError()) {
    SyncEvoStatic::reportDBusError(QString(__PRETTY_FUNCTION__), reply.error());
    m_error = true;
  }
}

/*
 * Upon StartSession, we perform the next step, which (currently always) is SetConfig. D-Bus call.
 * The use cases currently handled:
 * 1. Sync:
 *    StartSession -> SetConfig(m_config) -> Sync -> Detach (when the sync completes, not when the Sync call returns)
 * 2. RememberAutoSync:
 *    StartSession -> SetConfig(m_config, after having updated its autoSync flag) -> Detach
 * 3. Forget:
 *    StartSession -> SetConfig(empty config) -> Detach
 */
void
MeeGo::Sync::SyncEvoFrameworkClient::handleStartSession(QDBusPendingCallWatcher *call)
{
  QDBusPendingReply<QDBusObjectPath> reply = *call;

  if (reply.isError()) {
    SyncEvoStatic::reportDBusError(QString(__PRETTY_FUNCTION__), reply.error());
    m_error = true;
  }
  else
  if (!m_error) {
    /* Delete the old session interface, because it is useless, and create a new one */
    if (m_sessionInterface)
      delete m_sessionInterface;
    m_sessionInterface = new OrgSyncevolutionSessionInterface("org.syncevolution", reply.argumentAt<0>().path(), QDBusConnection::sessionBus(), this);
    connect(
      m_sessionInterface,
      SIGNAL(StatusChanged(const QString &, uint, const QSyncStatusMap &)),
      this,
      SLOT(sessionStatusChanged(const QString &, uint, const QSyncStatusMap &)));

    /*
     * FIXME: Magic value we need to pass
     */
    if (!IS_LOCAL_CONFIG(m_config))
      m_config[""]["preventSlowSync"] = "0";

    if (sessionActions.head() == SaveWebDAVLoginInfo) {
      SyncEvoStatic::dbusCall(
        QList<QProperty>()
          << QProperty("DBusFunctionName", "GetConfig")
          << QProperty("WebDAVConfig", true)
          << QProperty("ConfigName", m_name),
        this, SLOT(asyncCallFinished(QDBusPendingCallWatcher *)),
        m_sessionInterface->GetConfig(false));
    }
    else {
      /*
       * If we're not saving WebDAV log info, we call SetConfig before doing anything else with the session, passing in an
       * empty config when attempting to forget, and the current config otherwise
       */
      SyncEvoStatic::dbusCall(
        QList<QProperty>()
          << QProperty("DBusFunctionName", "SetConfig")
          << QProperty("ConfigName", m_name),
        this, SLOT(asyncCallFinished(QDBusPendingCallWatcher *)),
        m_sessionInterface->SetConfig(false, false,
          Forget == sessionActions.head()
            ? QStringMultiMap()
            : m_config));
    }
  }
}

/*
 * Upon Detach, the session has become useless, so move on to the next action - unless this is the end of a WebDAV
 * config saving session, in which case, continue with the same action.
 */
void
MeeGo::Sync::SyncEvoFrameworkClient::handleSessionDetach(QDBusPendingCallWatcher *call)
{
  /* WebDAV mode requires a second session for accomplishing the same thing */
  if (call->property("WebDAVConfig").isValid() && call->property("WebDAVConfig").toBool()) {
    if (m_config[""].contains("peerType") && "WebDAV" == m_config[""]["peerType"]) {
      m_config = makeLocalConfig();
    }
    m_inProgress = false;
    performAction();
  }
  else
    nextAction();
}

/*
 * Define a local configuration
 */
QStringMultiMap
MeeGo::Sync::SyncEvoFrameworkClient::makeLocalConfig()
{
  QStringMultiMap newConfig;

  newConfig[""]["ConsumerReady"] = "1";
  newConfig[""]["PeerIsClient"] = "1";
  newConfig[""]["PeerName"] = m_service;
  newConfig[""]["syncURL"] = "local://@" + m_name.toLower();

  /* Copy all the sources from the config and convert them to syncevolution sources */
  QMapIterator<QString, QStringMap> itr(m_config);
  while(itr.hasNext()) {
    itr.next();
    if (itr.key().startsWith("source/")) {
      QString sourceType = itr.key().right(itr.key().length() - 7 /* strlen("source/") */);

      newConfig[itr.key()]["backend"] = sourceType;
      newConfig[itr.key()]["sync"] = "two-way";
      newConfig[itr.key()]["uri"] = sourceType;
    }
  }

  return newConfig;
}

/*
 * Dispatcher for asynchronous D-Bus calls
 */
void
MeeGo::Sync::SyncEvoFrameworkClient::asyncCallFinished(QDBusPendingCallWatcher *call)
{
  if (call->property("ConfigName").toString() == m_name) {
    if ("GetConfig" == DBUS_CALL_FUNCTION_NAME(call))
      handleGetConfig(call);
    else
    if ("GetReports" == DBUS_CALL_FUNCTION_NAME(call))
      handleGetReports(call);
    else
    if ("SetConfig" == DBUS_CALL_FUNCTION_NAME(call))
      handleSetConfig(call);
    else
    if ("StartSession" == DBUS_CALL_FUNCTION_NAME(call))
      handleStartSession(call);
    else
    if ("Abort" == DBUS_CALL_FUNCTION_NAME(call))
      handleAbort(call);
    else
    if ("Session::Detach" == DBUS_CALL_FUNCTION_NAME(call))
      handleSessionDetach(call);
  }
  call->deleteLater();
}

/*
 * Update the UI to reflect status changes during sync
 */
void
MeeGo::Sync::SyncEvoFrameworkClient::sessionStatusChanged(const QString &status, uint error, const QSyncStatusMap &sources)
{
  Q_UNUSED(sources)

  /* Only interesting if something is going on */
  if (status != "idle") {
    /* If we're done ... */
    if ("done" == status) {
      /* ... the session is useless, so Detach */
      if (!m_error) {
        SyncEvoStatic::dbusCall(
          QList<QProperty>()
            << QProperty("DBusFunctionName", "Session::Detach")
            << QProperty("ConfigName", m_name),
          this, SLOT(asyncCallFinished(QDBusPendingCallWatcher *)),
          m_sessionInterface->Detach());
      }

      /* ... and decide on the status */
      if (0 == error)
        setStatus(tr("Sync completed"));
      else {
        /*
         * If there's an error, retrieve the newly created report, passing in the received error code so that, if the
         * retrieval is unsuccessful, a rudimentary report can be created locally from the error code. The status will
         * be calculated and set when the report retrieval completes (one way or the other).
         */
        if (!m_error)
          SyncEvoStatic::dbusCall(
            QList<QProperty>()
              << QProperty("DBusFunctionName", "GetReports")
              << QProperty("ConfigName", m_name)
              << QProperty("Error", error),
            this, SLOT(asyncCallFinished(QDBusPendingCallWatcher *)),
            m_serverInterface->GetReports(m_name, 0, 1));
      }
    }
    else
      /* ... we're not yet done syncing */
      setStatus(tr("Syncing now..."));
  }
}

/*
 * Common code between the performAction(SessionAction) and nextAction().
 * Ask for a new session, if all the conditions are right.
 */
void
MeeGo::Sync::SyncEvoFrameworkClient::performAction()
{
  if (!(m_inProgress || m_error || m_name.isEmpty())) {
    m_inProgress = true;

    QString sessionName = 
      sessionActions.head() == SaveWebDAVLoginInfo
        ? "source-config" + m_config[""]["syncURL"].right(m_config[""]["syncURL"].length() - 8/*strlen("local://")*/)
        : sessionActions.head() == Forget
          ? IS_LOCAL_CONFIG(m_config)
            ? "source-config" + m_config[""]["syncURL"].right(m_config[""]["syncURL"].length() - 8/*strlen("local://")*/)
            : m_name
          : (m_config[""].contains("peerType") && "WebDAV" == m_config[""]["peerType"])
            ? "source-config@" + m_name.toLower()
            : m_name;

    SyncEvoStatic::dbusCall(
      QList<QProperty>()
        << QProperty("DBusFunctionName", "StartSession")
        << QProperty("ConfigName", m_name),
      this, SLOT(asyncCallFinished(QDBusPendingCallWatcher *)),
      m_serverInterface->StartSession(sessionName));
  }
}

/*
 * Enqueue an action, and perform it immediately if there's currently nothing going on.
 */
void
MeeGo::Sync::SyncEvoFrameworkClient::performAction(SessionAction action)
{
  if (Sync == action && IS_LOCAL_CONFIG(m_config))
    sessionActions.enqueue(SaveWebDAVLoginInfo);
  sessionActions.enqueue(action);
  performAction();
}

/*
 * Dequeue the last action, and perform the next one, if there are any left.
 */
void
MeeGo::Sync::SyncEvoFrameworkClient::nextAction()
{
  if (sessionActions.count() > 0)
    sessionActions.dequeue();
  m_inProgress = false;
  if (sessionActions.count() > 0)
    performAction();
}

/*
 * Forget this configuration. If there's a sync in progress, it will be Abort()-ed.
 */
void
MeeGo::Sync::SyncEvoFrameworkClient::forgetProfile()
{
  /* Abort(), if there's currently a sync going on */
  if (!m_error && m_inProgress && Sync == sessionActions.head()) {
    SyncEvoStatic::dbusCall(
      QList<QProperty>()
        << QProperty("DBusFunctionName", "Abort")
        << QProperty("ConfigName", m_name),
      this, SLOT(asyncCallFinished(QDBusPendingCallWatcher *)),
      m_sessionInterface->Abort());
  }

  performAction(Forget);
}

/*
 * If the server changes hands or disappears, we must stop using it, because it will have forgotten all our state.
 */
void
MeeGo::Sync::SyncEvoFrameworkClient::serviceOwnerChanged(const QString &name, const QString &oldOwner, const QString &newOwner)
{
  Q_UNUSED(name)
  Q_UNUSED(oldOwner)
  Q_UNUSED(newOwner)

  if (name == m_serverDBusName) {
    emit serviceHasDied();
    m_error = true;
  }
}
