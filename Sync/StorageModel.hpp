/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef MEEGO_SYNC_STORAGE_MODEL_HPP
#define MEEGO_SYNC_STORAGE_MODEL_HPP

#include <SyncClientInterface.h>
#include <QAbstractListModel>
#include <QMap>

#include "ServiceInfo.hpp"

namespace Buteo
{
  class SyncProfile;
}

namespace MeeGo
{
  namespace Sync
  {
    /**
     * @class StorageModel
     *
     * @brief Sync Service/Storage list generator.
     *
     * List model used during population of the sync profile list
     * presented in ther sync UI/applet.
     */
    class StorageModel : public QAbstractListModel
    {
      Q_OBJECT

    public:

      typedef QList<ServiceInfo> service_list_type;

      enum ProfileRoles {
	/// Display name of the sync profile.
	DisplayNameRole  = Qt::UserRole + 1,

	/// Sync profile Name/ID.
	NameRole,
	/// Service icon image associated with the sync profile.
	ImageRole,

	/// Whether or not the sync profile is has been scheduled
	/// for automatic syncing.
	ActiveRole,

	/// Username associated with the sync provider.
	/// (e.g. Google).
	UsernameRole,

	/// Password associated with the sync provider.
	PasswordRole,

	/// Calendar, Contacts, etc.
	StorageRole,

	/// Whether or not a given Sync service item in the list is
	/// ready for use.
	Ready
      };

      StorageModel(char const * storage_names[], QObject * parent = 0);
      virtual ~StorageModel();

      // The usual list model methods.
      virtual int rowCount(QModelIndex const & /* parent */) const;
      virtual QVariant data(QModelIndex const & index, int role) const;

    private slots:

      /// Detect credentials retrieval completion.
      void serviceReady(QString id);

      /// Detect profile changes.
      void profileChanged(QString id, int type, QString profile);

    private:

      Q_DISABLE_COPY(StorageModel)

      /// Create list of objects containing visible sync profiles,
      /// where the profiles are stored in memory as fully fledged C++
      /// objects, not XML.
      void createServiceInfoList(char const * storage_names[]);

    private:

      /// The underlying sync engine (Buteo) client interface.
      Buteo::SyncClientInterface m_sci;

      /// List of "visible" sync services.
      service_list_type m_services;

    };

  }
}


#endif /* MEEGO_SYNC_STORAGE_MODEL_HPP */
