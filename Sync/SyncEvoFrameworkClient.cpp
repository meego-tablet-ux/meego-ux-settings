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

#define IS_WEBDAV_CONFIG(c) \
  (((c).contains("")) && \
   ((c)[""].contains("peerType")) && \
   ((c)[""]["peerType"] == "WebDAV"))

MeeGo::Sync::SyncEvoFrameworkClient::SyncEvoFrameworkClient(QObject* parent)
  : QObject(parent)
  , m_serverInterface(new OrgSyncevolutionServerInterface("org.syncevolution", "/org/syncevolution/Server", QDBusConnection::sessionBus(), this))
  , m_sessionInterface(0)
  , m_inProgress(false)
  , m_error(false)
  , m_scheduled(false)
  , m_status()
  , m_statusIsMasked(true)
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
    SyncEvoStatic::reportDBusError(__PRETTY_FUNCTION__ + QString(": Fatal error: Attach() failed: "), reply.error());
    m_error = true;
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

    performAction(GetInitialStatus);
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
  QString ret = m_status;

  if (m_statusIsMasked)
    ret = "";

  return ret;
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

/*
 * This function is not really used
 */
void
MeeGo::Sync::SyncEvoFrameworkClient::setStorage(QString s)
{
  Q_UNUSED(s)
}

QString
MeeGo::Sync::SyncEvoFrameworkClient::name() const
{
  return m_name;
}

/*
 * Acquire the data identifying what config/source the user wishes to sync from the model
 */
void
MeeGo::Sync::SyncEvoFrameworkClient::setName(QString s)
{
  QStringList ls = s.split(QString(QChar('\0')));
  if (2 == ls.count()) {
    if (ls[0] != m_name)
      m_name = ls[0];

    if (ls[1] != m_storage)
      m_storage = ls[1];

    /*
     * Once we know the service name (== the config in syncevo lingo) and the storage (== the source) we can get the
     * detais and the last known status
     */
    if (!m_error) {
      SyncEvoStatic::dbusCall(
        QList<QProperty>()
          << QProperty("DBusFunctionName", "GetConfig")
          << QProperty("ConfigName", m_name),
        this, SLOT(asyncCallFinished(QDBusPendingCallWatcher *)),
        m_serverInterface->GetConfig(m_name, false));

      /*
       * Has to be done sync so the report is available for when lastSyncTime() is called by the UI - otherwise
       * we don't have a fuzzyTime :(
       */
      SyncEvoStatic::dbusCall(
        QList<QProperty>()
          << QProperty("DBusFunctionName", "GetReports")
          << QProperty("ConfigName", m_name),
        this, SLOT(asyncCallFinished(QDBusPendingCallWatcher *)),
        m_serverInterface->GetReports(m_name, 0, 1))->waitForFinished();
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
MeeGo::Sync::SyncEvoFrameworkClient::timeFromReport(const QStringMap &report)
{
  QDateTime ret = QDateTime();

  if (report.contains("end"))
    ret = QDateTime::fromMSecsSinceEpoch(((qint64)report["end"].toUInt()) * 1000);

  return ret;
}

QDateTime
MeeGo::Sync::SyncEvoFrameworkClient::lastSyncTime()
{
  QDateTime ret = timeFromReport(m_lastReport);

  return ret;
}

void
MeeGo::Sync::SyncEvoFrameworkClient::setFuzzyTime(QString fuzzyTime)
{
  setStatusFromLastReport(fuzzyTime);
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
MeeGo::Sync::SyncEvoFrameworkClient::setStatusFromLastReport(const QString &fuzzyTime, bool initialStatus)
{
  if (!fuzzyTime.isEmpty())
    m_fuzzyTime = fuzzyTime;

  //: Status message: Displayed when an unknown/unhandled error occurs.
  QString statusMessage = tr("Unknown sync status");

  if (m_lastReport.contains("status")) {
    int status = m_lastReport["status"].toUInt();

    if (200 == status && initialStatus) {
      if (m_fuzzyTime.isEmpty()) {
        emit statusChanged(m_status);
        return;
      }
    }

    statusMessage =
      (200 == status)
        //: Arg 1 is a "fuzzy time", e.g. "2 min ago".
        ? tr("Last sync %1").arg(
          m_fuzzyTime.isEmpty()
            ? lastSyncTime().toString("yyyy-MM-dd hh:mm:ss.zzz")
            : m_fuzzyTime)
        :
      (20017 == status)
        //: Sync explicitly stopped.
        ? tr("Sync aborted")
        //: Arg 1 is a brief sync error description, e.g. "connection failed".
        : tr("Last sync failed: %1").arg(
            (401 == status || 403 == status || 10401 == status || 10403 == status)
              //: Authentication failure occurred during sync.
              ? tr("authentication failure")
              //: Internal error occurred during sync.
              : tr("internal error"));
  }

  if (initialStatus && statusMessage == m_status)
    emit statusChanged(m_status);
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
    SyncEvoStatic::reportDBusError(QString(__PRETTY_FUNCTION__) + ": Fatal error: ", reply.error());
    m_error = true;
  }
}

/*
 * Upon GetConfig, we try to retrieve the template configuration, if the non-template configuration is absent.
 * Once retrieved, we set the name, username, password, and scheduled properties. If username and/or password are
 * absent, we emit the authenticationFailed() signal, which causes a login dialog to be popped, giving us the
 * opportunity to acquire a username/password from the user.
 *
 * If GetConfig was called as part of a SaveWebDAVLoginInfo action, the configuration we have received is the
 * source configuration, and its username and password fields must be updated, and the updated configuration
 * must be written back.
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
      if (call->property("ContextConfig").isValid() && call->property("ContextConfig").toBool())
        SyncEvoStatic::dbusCall(
          QList<QProperty>()
            << QProperty("DBusFunctionName", "GetConfig")
            << QProperty("ContextConfig", true)
            << QProperty("ConfigName", m_name)
            << QProperty("TriedTemplate", true),
          this, SLOT(asyncCallFinished(QDBusPendingCallWatcher *)),
          m_serverInterface->GetConfig(m_name.toLower() + "@" + m_name.toLower(), true));
      else
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
      qWarning() << QString(__PRETTY_FUNCTION__) + ": Fatal error: emitting profileRemoved(" + m_name + ")";
      emit profileRemoved(m_name);
      m_error = true;
    }
  }
  else {
    QStringMultiMap theConfig = reply.argumentAt<0>();

    if (!(call->property("WebDAVConfig").isValid() && call->property("WebDAVConfig").toBool())) {
      if (call->property("CreateLocalConfig").isValid() && call->property("CreateLocalConfig").toBool()) {
        m_config = makeLocalConfig(theConfig);
        m_inProgress = false;
        performAction();
        return;
      }
      else
        m_config = reply.argumentAt<0>();
    }

    /* Check if this is a local config and, if so, retrieve the source config */
    if (IS_LOCAL_CONFIG(theConfig)) {

      QString sourceConfig =
        "target-config" + 
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
    if (IS_WEBDAV_CONFIG(theConfig) &&
        !(call->property("WebDAVConfig").isValid() && call->property("WebDAVConfig").toBool()) &&
        !(call->property("ContextConfig").isValid() && call->property("ContextConfig").toBool())) {
      SyncEvoStatic::dbusCall(
        QList<QProperty>()
          << QProperty("DBusFunctionName", "GetConfig")
          << QProperty("ContextConfig", true)
          << QProperty("ConfigName", m_name),
        this, SLOT(asyncCallFinished(QDBusPendingCallWatcher *)),
        m_serverInterface->GetConfig(m_name.toLower() + "@" + m_name.toLower(), false));
    }
    else 
    if (!sessionActions.isEmpty() && sessionActions.head() == SaveWebDAVLoginInfo) {
      /* Update WebDAV login information and re-save the source config */
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
  bool initialStatus = 
    (sessionActions.count() > 0 && 
     GetInitialStatus == sessionActions.head() &&
     call->property("InitialStatus").isValid() && 
     call->property("InitialStatus").toBool() == true);
  QDBusPendingReply<QArrayOfStringMap> reply = *call;

  /* Clear the last report */
  m_lastReport.clear();

  if (reply.isError())
    SyncEvoStatic::reportDBusError(QString(__PRETTY_FUNCTION__), reply.error());
  else {
    QArrayOfStringMap reports = reply.argumentAt<0>();

    if (reports.count() > 0) {
      QDateTime oldLastSyncTime = timeFromReport(m_lastReport);
      QDateTime newLastSyncTime = timeFromReport(reports[0]);
      m_lastReport = reports[0];
      if (newLastSyncTime != oldLastSyncTime)
        emit lastSyncTimeChanged(newLastSyncTime);
    }
  }

  /* Even if the report retrieval fails, the PendingCallWatcher may have an "Error" field.
   * If so, and the report doesn't have a status field, create one from the "Error" field. */
  if (!m_lastReport.contains("status") && call->property("Error").isValid())
    m_lastReport["status"] = call->property("Error").toString();

  
  if (initialStatus) {
    m_statusIsMasked = false;
    if (!m_error)
      SyncEvoStatic::dbusCall(
        QList<QProperty>()
          << QProperty("DBusFunctionName", "Session::Detach")
          << QProperty("ConfigName", m_name),
        this, SLOT(asyncCallFinished(QDBusPendingCallWatcher *)),
        m_sessionInterface->Detach());
  }

  setStatusFromLastReport(QString(), initialStatus);

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
 * SetConfig is called at the start of any session-based activity. There are currently 4 session-based tasks:
 *
 * 1. When forgetting a profile, the config is set to blank (QStringMultiMap()).
 * 2. When a sync is performed, SetConfig is called first, to save the possible username/password update.
 * 3. When autoSync is toggled, the autoSync field must be saved.
 * 4. Before performing a sync on a WebDAV-based configuration, the username and the password must be saved to the
 * source config.
 *
 * The call watcher's "ErrorMessage" property is used in case of a D-Bus error. It depends on which of the above four
 * reasons has caused this call and may be set when making this call.
 */
void
MeeGo::Sync::SyncEvoFrameworkClient::handleSetConfig(QDBusPendingCallWatcher *call)
{
  QDBusPendingReply<> reply = *call;

  if (reply.isError()) {
    SyncEvoStatic::reportDBusError(QString(__PRETTY_FUNCTION__) + "Fatal error: ", reply.error());
    m_error = true;

    switch (sessionActions.head()) {
      case Forget:
        //: Displayed when removal of sync account information fails.
        setStatus(tr("Unable to forget sync account!"));
        break;

      case Sync:
      case SaveWebDAVLoginInfo:
        //: Sync explicitly stopped.
        setStatus(tr("Sync aborted"));
        break;

      default:
        break;
    }
  }
  else {
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
        break;

      case Sync:
        if (IS_WEBDAV_CONFIG(m_config)) {
          detachProps << QProperty("WebDAVConfig", true);
        }
        else
        if (!m_error) {
          QStringMap whatToSync;
          QStringList ls = m_storage.split("/");

          if (m_config[m_storage].contains("sync") && 2 == ls.count()) {
            whatToSync[ls[1]] = m_config[m_storage]["sync"];

            SyncEvoStatic::dbusCall(
              QList<QProperty>()
                << QProperty("DBusFunctionName", "Sync")
                << QProperty("ConfigName", m_name),
              this, SLOT(asyncCallFinished(QDBusPendingCallWatcher *)),
              m_sessionInterface->Sync("none", whatToSync));

            /* Do not detach from session until sync is complete */
            return;
          }
        }
        break;

      default:
        break;
    }

    if (!m_error)
      SyncEvoStatic::dbusCall(detachProps,
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
    SyncEvoStatic::reportDBusError(QString(__PRETTY_FUNCTION__) + ": Fatal error: ", reply.error());
    m_error = true;
  }
}

void
MeeGo::Sync::SyncEvoFrameworkClient::handleGetStatus(QDBusPendingCallWatcher *call)
{
  QDBusPendingReply<QString, uint, QSyncStatusMap> reply = *call;

  if (reply.isError()) {
    SyncEvoStatic::reportDBusError(QString(__PRETTY_FUNCTION__) + ": Fatal error: ", reply.error());
    m_error = true;
  }
  else
    sessionStatusChanged(reply.argumentAt<0>(), reply.argumentAt<1>(), reply.argumentAt<2>());
}

/*
 * Upon StartSession, we perform the next step, which is SetConfig, unless we're starting a WebDAV-based Sync, in
 * which case we first save the username/password to the source config, and then start a second session for the actual
 * sync.
 * The use cases currently handled:
 * 1. Sync:
 *    StartSession -> SetConfig(m_config) -> Sync -> Detach (when the sync completes, not when the Sync call returns)
 * 2. RememberAutoSync:
 *    StartSession -> SetConfig(m_config, after having updated its autoSync flag) -> Detach
 * 3. Forget:
 *    StartSession -> SetConfig(empty config) -> Detach
 * 4. SaveWebDAVLoginInfo:
 *    StartSession -> GetConfig -> SetConfig(retrieved config + username/password) -> Detach
 */
void
MeeGo::Sync::SyncEvoFrameworkClient::handleStartSession(QDBusPendingCallWatcher *call)
{
  QDBusPendingReply<QDBusObjectPath> reply = *call;

  if (reply.isError()) {
    SyncEvoStatic::reportDBusError(QString(__PRETTY_FUNCTION__) + ": Fatal error: ", reply.error());
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

    SyncEvoStatic::dbusCall(
      QList<QProperty>()
        << QProperty("DBusFunctionName", "GetStatus")
        << QProperty("ConfigName", m_name),
      this, SLOT(asyncCallFinished(QDBusPendingCallWatcher *)),
      m_sessionInterface->GetStatus());
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
    if (IS_WEBDAV_CONFIG(m_config)) {
      if (!m_error)
        SyncEvoStatic::dbusCall(
          QList<QProperty>()
            << QProperty("DBusFunctionName", "GetConfig")
            << QProperty("CreateLocalConfig", true)
            << QProperty("TriedTemplate", true)
            << QProperty("ConfigName", m_name),
          this, SLOT(asyncCallFinished(QDBusPendingCallWatcher *)),
          m_serverInterface->GetConfig("SyncEvolution_client", true));
    }
    else {
      m_inProgress = false;
      performAction();
    }
  }
  else
    nextAction();
}

/*
 * Define a local configuration based ono the SyncEvolution_client@default configuration.
 */
QStringMultiMap
MeeGo::Sync::SyncEvoFrameworkClient::makeLocalConfig(const QStringMultiMap &templateConfig)
{
  QStringMultiMap newConfig = templateConfig;

  newConfig[""]["ConsumerReady"] = "1";
  newConfig[""]["PeerName"] = m_service;
  newConfig[""]["syncURL"] = "local://@" + m_name.toLower();
  if (m_config[""].contains("IconURI"))
    newConfig[""]["IconURI"] = m_config[""]["IconURI"];

  if (newConfig[""].contains("username"))
    newConfig[""].remove("username");

  if (newConfig[""].contains("password"))
    newConfig[""].remove("password");

  QMutableMapIterator<QString, QStringMap> itr(newConfig);
  while (itr.hasNext()) {
    itr.next();
    if (itr.key().startsWith("source/")) {
      if (!m_config.contains(itr.key())) {
        itr.remove();
      }
      else {
        if (m_config[itr.key()].contains("sync")) {
          QStringMap templateSource = itr.value();
          templateSource["sync"] = m_config[itr.key()]["sync"];
          itr.setValue(templateSource);
        }
        else
        if (itr.value().contains("sync")) {
          QStringMap templateSource = itr.value();
          templateSource.remove("Sync");
          itr.setValue(templateSource);
        }
      }
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
  if (call->property("ConfigName").toString() == m_name ||
      (call->property("InitialStatus").isValid() && call->property("InitialStatus").toBool())) {
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
    else
    if ("GetStatus" == DBUS_CALL_FUNCTION_NAME(call))
      handleGetStatus(call);
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

  if (!m_sessionIsReady && status == "idle") {
    // Session has become ready for action
    m_sessionIsReady = true;

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
    else
    if (sessionActions.count() > 0 && sessionActions.head() == GetInitialStatus) {
      if (m_name.isEmpty()) {
        m_statusIsMasked = false;
        emit statusChanged(m_status);
        if (!m_error)
          SyncEvoStatic::dbusCall(
            QList<QProperty>()
              << QProperty("DBusFunctionName", "Session::Detach")
              << QProperty("ConfigName", m_name),
            this, SLOT(asyncCallFinished(QDBusPendingCallWatcher *)),
            m_sessionInterface->Detach());
      }
      else
      if (!m_error) {
        SyncEvoStatic::dbusCall(
          QList<QProperty>()
            << QProperty("DBusFunctionName", "GetReports")
            << QProperty("InitialStatus", true)
            << QProperty("ConfigName", m_name),
          this, SLOT(asyncCallFinished(QDBusPendingCallWatcher *)),
          m_serverInterface->GetReports(m_name, 0, 1));
      }
    }
    else {
      /* When saving the WebDAV configuration, we don't want it showing up in the UI later, so make it invisible. */
      if (IS_WEBDAV_CONFIG(m_config))
        m_config[""]["ConsumerReady"] = "0";

      /*
       * If we're not saving WebDAV log info, we call SetConfig before doing anything else with the session, passing in an
       * empty config when attempting to forget, and the current config otherwise.
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
  else
  /* Only interesting if something is going on */
  if (status != "idle") {
    //: Status message: Sync in-progress
    QString displayStatus = tr("Syncing now...");

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
        //: Status message: Sync job completed
        displayStatus = tr("Sync completed");
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
    setStatus(displayStatus);
  }
}

/*
 * Common code between the performAction(SessionAction) and nextAction().
 * Ask for a new session, if all the conditions are right.
 */
void
MeeGo::Sync::SyncEvoFrameworkClient::performAction()
{
  if (!(m_inProgress || m_error)) {
    QString sessionName;

    if (m_name.isEmpty() && sessionActions.head() == GetInitialStatus)
      sessionName = "";
    else
      sessionName = 
        sessionActions.head() == SaveWebDAVLoginInfo
          ? "target-config" + m_config[""]["syncURL"].right(m_config[""]["syncURL"].length() - 8/*strlen("local://")*/)
          :
        sessionActions.head() == Forget
          ? IS_LOCAL_CONFIG(m_config)
            ? "target-config" + m_config[""]["syncURL"].right(m_config[""]["syncURL"].length() - 8/*strlen("local://")*/)
            : m_name
          : (IS_WEBDAV_CONFIG(m_config))
            ? "target-config@" + m_name.toLower()
            : m_name;

    if (!sessionName.isNull()) {
      m_inProgress = true;
      m_sessionIsReady = false;

      SyncEvoStatic::dbusCall(
        QList<QProperty>()
          << QProperty("DBusFunctionName", "StartSession")
          << QProperty("InitialStatus", (sessionName == ""))
          << QProperty("ConfigName", m_name),
        this, SLOT(asyncCallFinished(QDBusPendingCallWatcher *)),
        m_serverInterface->StartSession(sessionName));
    }
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
