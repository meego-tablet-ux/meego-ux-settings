/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef MEEGO_SYNC_SYNC_EVO_FW_CLIENT_HPP
#define MEEGO_SYNC_SYNC_EVO_FW_CLIENT_HPP

#include <QDateTime>
#include <QDBusError>
#include <QObject>
#include <QQueue>
#include <syncevolution-qt-dbus/dbustypes.h>

class QDBusPendingCallWatcher;
class OrgSyncevolutionServerInterface;
class OrgSyncevolutionSessionInterface;

namespace MeeGo {
  namespace Sync {
    /**
     * @class FrameworkClient
     *
     * @brief FrameworkClient allows QML-based manipulation of sync
     *        operations.
     *
     * The FrameworkClient allows a QML based application to perform
     * common syncing operations, such as sync profile creation,
     * starting a sync, retrieving the sync status, etc.  Each
     * instance operates on a single sync profile.
     *
     * @todo This class is currently designed to only support online
     *       sync services (Google, Yahoo, etc).
     */
    class SyncEvoFrameworkClient : public QObject
    {
      Q_OBJECT
      Q_PROPERTY(bool scheduled READ scheduled WRITE setScheduled NOTIFY scheduledChanged)
      Q_PROPERTY(QString status READ status NOTIFY statusChanged)
      Q_PROPERTY(QString service READ service WRITE setService)
      Q_PROPERTY(QString storage READ storage WRITE setStorage)
      Q_PROPERTY(QString name READ name WRITE setName)
      Q_PROPERTY(QString username READ username WRITE setUsername NOTIFY usernameChanged)
      Q_PROPERTY(QString password READ password WRITE setPassword NOTIFY passwordChanged)
      Q_PROPERTY(QDateTime lastSyncTime READ lastSyncTime NOTIFY lastSyncTimeChanged)

    public:

      SyncEvoFrameworkClient(QObject* parent = 0);
      virtual ~SyncEvoFrameworkClient();

      /// Recurring sync scheduled?
      bool scheduled() const;

      /// Retrieve stringified version of active sync profiles
      /// current sync status, suitable for display in the UI.
      QString status() const;

      /// Set the sync status string.
      void setStatus(QString s);

      /// Retrieve sync service name (e.g. "Google").
      QString service() const;

      /// Set sync service name (e.g. "Google").
      void setService(QString s);

      /// Retrieve sync storage name (e.g. "Calendar").
      QString storage() const;

      /// Set sync storage name (e.g. "Calendar").
      void setStorage(QString s);

      /// Retrieve sync profile name.
      QString name() const;

      /// Set sync profile name.
      void setName(QString s);

      /// Retrieve sync username.
      QString username() const;

      /// Set sync username.
      void setUsername(QString s);

      /// Retrieve sync password.
      QString password() const;

      /// Set sync password
      void setPassword(QString s);

      /// Get the last time a sync occured.
      QDateTime lastSyncTime();

      // Set the status message to the last time a sync occurred, or
      // perform an initial sync if one has not been run.
      // @note We pass the fuzzy last sync time from QML since it is
      //       currently not straightforward to get the fuzzy time
      //       directly through a C++ call.
      Q_INVOKABLE void doPostInit(QString fuzzyTime,
				  bool forceSync);

      Q_INVOKABLE void setFuzzyTime(QString fuzzyTime);

      /// Explicitly run a sync now.
      Q_INVOKABLE void syncNow();

      /// Enable or disable automated syncing.
      Q_INVOKABLE void enableAutoSync(bool enable);

      /// Remove profile from disk, and cleanup SSO registration.
      Q_INVOKABLE void forgetProfile();

    signals:

      /// Notify of changes to the last sync time
      void lastSyncTimeChanged(QDateTime s);

      /// Signal emitted when the sync 'scheduled' flag has
      /// changed.
      void scheduledChanged(bool s);

      /// Signal emitted when the sync status has changed.
      void statusChanged(QString s);

      /// Signal emitted when the sync account username has changed.
      void usernameChanged(QString s);

      /// Signal emitted when the sync account password has changed.
      void passwordChanged(QString s);

      /// Signal emitted when the profile is removed.
      void profileRemoved(QString id);

      /// Signal emitted upon sync account login authentication failure.
      void authenticationFailed();

      /// Signal emitted when the underlying backend has died.
      void serviceHasDied();

    private slots:
      /// status report from session
      void sessionStatusChanged(const QString &status, uint error, const QSyncStatusMap &sources);

      /// asynchronous D-Bus call finished
      void asyncCallFinished(QDBusPendingCallWatcher *watcher);

      /// If the server dies ...
      void serviceOwnerChanged(const QString &name, const QString &oldOwner, const QString &newOwner);
    private:

      /// Prevent copy construction and assignment.
      Q_DISABLE_COPY(SyncEvoFrameworkClient)

      /// Schedule and start the initial sync.
      void doInitialSync();

      /// Set the recurring sync property.
      void setScheduled(bool s);

      /// Update the status from the last available report. Use the "fuzzy time" if available.
      void setStatusFromLastReport(const QString &fuzzyTime = QString(), bool initalStatus = false);

      /// Create a local config for WebDAV based stuff.
      QStringMultiMap makeLocalConfig();

      /// asynchronous D-Bus call handlers dispatched from asyncCallFinished()
      void handleGetConfig(QDBusPendingCallWatcher *call);
      void handleGetReports(QDBusPendingCallWatcher *call);
      void handleStartSession(QDBusPendingCallWatcher *call);
      void handleSetConfig(QDBusPendingCallWatcher *call);
      void handleAbort(QDBusPendingCallWatcher *call);
      void handleSync(QDBusPendingCallWatcher *call);
      void handleSessionDetach(QDBusPendingCallWatcher *call);
      void handleGetStatus(QDBusPendingCallWatcher *call);

      /*
       * Since actions are performed asynchronously, a queue is used to keep track of what to do next.
       *
       * The queue contains values from the SessionAction enum, and is being filled by performAction(SessionAction action).
       * At the end of each action, "Detach" is called on the session interface, which, when complete, calls nextAction().
       */
      enum SessionAction {
        Sync,
        Forget,
        RememberAutoSync,
        SaveWebDAVLoginInfo,
        GetInitialStatus
      };

      void performAction(SessionAction action);
      void performAction();
      void nextAction();
      QString contextSessionName() const;
      QDateTime timeFromReport(const QStringMap &report);

    private:

      /// List of pending actions
      QQueue<SessionAction> sessionActions;

      /*
       * The underlying D-Bus interfaces.
       *
       * The server interface is created once and lives as long as the object itself.
       *
       * The session interface is created once for each action.
       * The previous session interface object is deleted if present.
       */
      OrgSyncevolutionServerInterface *m_serverInterface;
      OrgSyncevolutionSessionInterface *m_sessionInterface;

      //// Keeps actions sequential
      bool m_inProgress;

      /*
       * Configuration for this service
       * If this is a WebDAV configuration, then this will eventually hold the local configuration,
       * however, it will hold the WebDAV configuration (a.k.a source configuration) until the username
       * and password have been given and the local configuration has been created with makeLocalConfig().
       */
      QStringMultiMap m_config;

      /// The last retrieved report
      QStringMap m_lastReport;

      /// Whether or not there has been an error that prevents further operation.
      bool m_error;

      /// Whether or not a sync schedule has been set.
      bool m_scheduled;

      /// Stringified sync status.
      QString m_status;

      /// Whether the status is masked
      bool m_statusIsMasked;

      /// Sync service name (e.g. "Google").
      QString m_service;

      /// Sync storage name (e.g. "Calendar").
      QString m_storage;

      /// Sync profile name.
      QString m_name;

      /// Sync username.
      QString m_username;

      /// Sync password.
      QString m_password;

      /// Syncevo server D-Bus unique name
      QString m_serverDBusName;

      /// Whether the current session is ready for use
      bool m_sessionIsReady;

      /// Fuzzy time received from QML
      QString m_fuzzyTime;
    };
  }
}

#endif  /* !MEEGO_SYNC_SYNC_EVO_FW_CLIENT_HPP */
