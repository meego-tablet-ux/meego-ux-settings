/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef MEEGO_SYNC_FW_CLIENT_HPP
#define MEEGO_SYNC_FW_CLIENT_HPP

#include "SharedCredentials.hpp"

#include <QObject>
#include <SyncClientInterface.h>
#include <ProfileManager.h>
#include <QScopedPointer>


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
    class FrameworkClient : public QObject
    {
      Q_OBJECT
      Q_PROPERTY(bool scheduled READ scheduled NOTIFY scheduledChanged)
      Q_PROPERTY(QString status READ status NOTIFY statusChanged)
      Q_PROPERTY(QString service READ service WRITE setService)
      Q_PROPERTY(QString storage READ storage WRITE setStorage)
      Q_PROPERTY(QString name READ name WRITE setName)
      Q_PROPERTY(QString username READ username WRITE setUsername)
      Q_PROPERTY(QString password READ password WRITE setPassword)

    public:

      FrameworkClient(QObject* parent = 0);
      virtual ~FrameworkClient();

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
      Q_INVOKABLE QDateTime lastSyncTime();

      // Set the status message to the last time a sync occurred, or
      // perform an initial sync if one has not been run.
      // @note We pass the fuzzy last sync time from QML since it is
      //       currently not straightforward to get the fuzzy time
      //       directly through a C++ call.
      Q_INVOKABLE void doPostInit(QString fuzzyTime,
				  bool forceSync);

      /// Explicitly run a sync now.
      Q_INVOKABLE void syncNow();

      /// Enable or disable automated syncing.
      Q_INVOKABLE void enableAutoSync(bool enable);

      /// Remove profile from disk, and cleanup SSO registration.
      Q_INVOKABLE void forgetProfile();

    signals:

      /// Signal emitted when the sync 'scheduled' flag has
      /// changed.
      void scheduledChanged(bool s);

      /// Signal emitted when the sync status has changed.
      void statusChanged(QString s);

      /// Signal emitted when the profile is removed.
      void profileRemoved(QString id);

      /// Signal emitted upon sync account login authentication failure.
      void authenticationFailed();

    private slots:

      /// Sync status update occured.
      void syncStatus(QString id,
		      int statusCode,
		      QString message,
		      int details);

      /// Sync results available.
      void resultsAvailable(QString id,
			    Buteo::SyncResults results);

      /// Detect profile changes.
      void profileChanged(QString id, int type, QString profile);

      // ---

      /// Detect credentials storage.
      void credentialsStored(quint32 id);

      /// Detect credentials removal.
      void credentialsRemoved();

      /// Detect credentials related errors.
      void credentialsError(const SignOn::Error & e);

    private:

      /// Prevent copy construction and assignment.
      Q_DISABLE_COPY(FrameworkClient)

      /// Schedule and start the initial sync.
      void doInitialSync();

      /// Set the recurring sync property.
      void setScheduled(bool s);

      /// Get the identifier associated with the sync account
      /// credentials stored in the single sign-on framework.
      static QString getSsoCaption(QString const & name);

      /// Get string describing the sync error.
      static QString syncResultToString(Buteo::SyncResults const & results);

    private:

      /// The underlying sync engine (Buteo) client interface.
      Buteo::SyncClientInterface m_sci;

      /// Buteo's profile management interface.
      Buteo::ProfileManager m_pm;

      /// Credentials processing strategy currently in use.
      QScopedPointer<SharedCredentialsProcessor> m_processor;

      /// Credentials management object.
      QScopedPointer<SharedCredentials> m_cred;

      /// Whether or not a sync schedule has been set.
      bool m_scheduled;

      /// Stringified sync status.
      QString m_status;

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

      /// Sync provider (e.g. google.com).  May or may not be same as
      /// @c m_service.
      QString m_provider;

      /// Number of profiles with matching SSO provider to be removed.
      int m_removalsPending;

    };

  }
}

#endif  /* MEEGO_SYNC_FW_CLIENT_HPP */
