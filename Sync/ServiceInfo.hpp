/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef MEEGO_SYNC_SERVICE_INFO_HPP
#define MEEGO_SYNC_SERVICE_INFO_HPP

#include "RetrieveCredentialsProcessor.hpp"

#include <QScopedPointer>
#include <QString>

namespace Buteo
{
  class SyncProfile;
}

namespace SignOn
{
  class Error;
}

class SharedCredentials;

namespace MeeGo
{
  namespace Sync
  {
    class StorageModel;

    /**
     * @class ServiceInfo
     *
     * @brief Class that encapsulates remote sync service information.
     */
    class ServiceInfo : public QObject
    {

      Q_OBJECT

    public:

      /// Constructor.
      /**
       * @note This class assumes ownership of the profile.
       */
      ServiceInfo(Buteo::SyncProfile * profile,
		  StorageModel * model,
		  QObject * parent = 0);

      /// Copy constructor.
      ServiceInfo(ServiceInfo const & rhs);

      /// Copy assignment operator.
      void operator=(ServiceInfo const & rhs);

      /// Destructor.
      ~ServiceInfo();

      /// Get sync service name suitable for display in UI
      /// (e.g. "Google").
      QString displayName() const;

      /// Get sync service profile name (used internally).
      QString profileName() const;

      /// Get name of file containing icon associated with the given
      /// service.
      QString imageFilename() const;

      /// Is the sync service configured and set up for automatic
      /// syncing.
      bool active() const;

      /// Get the username used to log in to the remote sync service.
      QString username() const;

      /// Get the password used to log in to the remote sync service.
      QString password() const;

      /// Get the storage name associated with this sync service
      /// (e.g. "Calendar" or "Contacts").
      QString storage() const;

      /// Whether or not this object is ready for use.
      /**
       * The "readiness" of this object depends on the completion
       * status of operations that may be running asynchronously.
       */
      bool ready() const;

      /// Allow ServiceInfo objects to be sorted using standard
      /// algorithms.
      bool operator<(ServiceInfo const & rhs) const;

    signals:

      /// Signal that is triggered when this ServiceInfo object is
      /// ready for use.
      void serviceReady(QString id);

    private slots:

      /// Set username and password fields to the given username and
      /// password.
      void credentials(const QString & username, const QString & password);

      /// Error occurred when retrieving username and password.
      void error(const SignOn::Error &);

    private:

      /// Retrieve string value from sync profile.
      /**
       * @note Only the first value of many, if many exist, will be
       *       returned.
       */
      QString syncValue(QString const & key);

      /// Retrieve credentials associated with the remote sync
      /// service.  Retrieval may potentially occur asynchronously.
      void retrieveCredentials() const;

      /// Set 'ready' status, and emit the serviceReady() signal as
      /// needed.
      void setReady();

    private:

      /// The underlying sync profile object.
      QScopedPointer<Buteo::SyncProfile> m_profile;

      /// The model to which we belong, and will connect the
      /// @c serviceReady() signal to.
      StorageModel * m_model;

      /// Credentials retrieval object.
      mutable QScopedPointer<RetrieveCredentialsProcessor> m_processor;

      /// Credentials management.
      mutable QScopedPointer<SharedCredentials> m_cred;

      /// Sync storage name (e.g. "Contacts" or "Calendar").
      QString m_storage;

      /// Username associate with the remote sync service.
      /**
       * @note This field may be updated asynchronously.
       */
      QString m_username;

      /// Username associate with the remote sync service.
      /**
       * @note This field may be updated asynchronously.
       */
      QString m_password;

      /// True if this ServiceInfo object is ready for use.
      /**
       * @note This will only be false if an asynchronous credentials
       *       retrieval process is under way.
       */
      bool m_ready;

    };
  }
}


#endif  /* MEEGO_SYNC_SERVICE_INFO_HPP */
