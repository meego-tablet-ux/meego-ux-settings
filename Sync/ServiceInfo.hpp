/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef MEEGO_SYNC_SERVICE_INFO_HPP
#define MEEGO_SYNC_SERVICE_INFO_HPP


#include <QScopedPointer>
#include <QObject>


namespace Buteo
{
  class SyncProfile;
}

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

      /// Get the storage name associated with this sync service
      /// (e.g. "Calendar" or "Contacts").
      QString storage() const;

      /// Allow ServiceInfo objects to be sorted using standard
      /// algorithms.
      bool operator<(ServiceInfo const & rhs) const;

    private:

      /// The underlying sync profile object.
      QScopedPointer<Buteo::SyncProfile> m_profile;

      /// Sync storage name (e.g. "Contacts" or "Calendar").
      QString m_storage;

    };
  }
}


#endif  /* MEEGO_SYNC_SERVICE_INFO_HPP */
