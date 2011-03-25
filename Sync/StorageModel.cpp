/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include "StorageModel.hpp"

#include <SyncClientInterface.h>
#include <QDomDocument>
#include <QDebug>

#include <algorithm>


MeeGo::Sync::StorageModel::StorageModel(char const * storage_names[],
					QObject * parent)
  : QAbstractListModel(parent)
  , m_sci()
  , m_services()
{
  QHash<int, QByteArray> roles; 
  roles[DisplayNameRole] = "displayName";
  roles[NameRole]        = "name"; // "id" is an existing QML property.
  roles[ImageRole]       = "image";
  roles[ActiveRole]      = "active";
  roles[UsernameRole]    = "username";
  roles[PasswordRole]    = "password";
  roles[StorageRole]     = "storage";
  roles[Ready]           = "ready";
  setRoleNames(roles);

  // Construct list of visible sync services.
  createServiceInfoList(storage_names);

  // Detect sync profile changes.
  connect(&m_sci,
	  SIGNAL(profileChanged(QString,int,QString)),
	  this,
	  SLOT(profileChanged(QString,int,QString)));
}

MeeGo::Sync::StorageModel::~StorageModel()
{
}

void
MeeGo::Sync::StorageModel::serviceReady(QString id)
{
  int row = 0;

  // Find the row containing ServiceInfo object with the updated
  // credentials.
  service_list_type::iterator const end = m_services.end();
  service_list_type::iterator i;
  for (i = m_services.begin(); i != end; ++i) {
    if ((*i).profileName() == id) {
      // Credentials have changed.  Update the QML ListView.
      emit dataChanged(index(row, 0), index(row, 0));

      return;
    }

    ++row;
  }
}

void
MeeGo::Sync::StorageModel::profileChanged(QString id,
					  int type,
					  QString profile_xml)
{
  // We only expect profile modification and removal changes.
  if (type == 0 || type > 2) {
    qDebug() << "INFO: Ignoring sync profile change type ("
	       << type << ") for \"" << id << "\".";
    return;
  }

  int row = 0;

  // Update the profile matching the given ID.
  service_list_type::iterator const end = m_services.end();
  service_list_type::iterator i;
  for (i = m_services.begin(); i != end; ++i) {
    ServiceInfo & info = *i;

    if (info.profileName() == id) {
      QDomDocument doc;

      if (type == 2 /* Buteo::ProfileManager::PROFILE_REMOVED */
	  || profile_xml.isEmpty()
	  || profile_xml.isNull()) {
	// We don't want to use the profile passed to this slot in
	// the deleted profile case since it is empty.  Reload the
	// vanilla profile from disk.
	//
	// Note that the one in the primary (home) sync profile
	// directory was removed.  This call will pull the sync
	// profile from system (secondary) sync profile directory,
	// which is the one that was originally copied when the
	// removed profile was first created.
	profile_xml = m_sci.syncProfile(id);
      }

      doc.setContent(profile_xml);

      ServiceInfo const tmp(new Buteo::SyncProfile(doc.documentElement()),
			    this);
      info = tmp;

      // Profile data has changed.  Update the QML ListView.
      emit dataChanged(index(row, 0), index(row, 0));

      return;
    }

    ++row;
  }
}

int
MeeGo::Sync::StorageModel::rowCount(QModelIndex const & /* parent */) const
{
  return m_services.size();
}

QVariant
MeeGo::Sync::StorageModel::data(QModelIndex const & index, int role) const
{
  int row = index.row();

  if (row < 0 || row >= m_services.size())
    return QVariant();

  ServiceInfo const & info = m_services[row];

  switch(role) {
  case DisplayNameRole:
    return info.displayName();

  case NameRole:
    return info.profileName();

  case ImageRole:
    return info.imageFilename();

  case ActiveRole:
    return info.active();

  case UsernameRole:
    return info.username();

  case PasswordRole:
    return info.password();

  case StorageRole:
    return info.storage();

  case Ready:
    return info.ready();

  default:
    break;
  }

  return QVariant();
}

void MeeGo::Sync::StorageModel::createServiceInfoList(
  char const * storage_names[])
{
  QList<QString> x(m_sci.allVisibleSyncProfiles());

  typedef QList<QString>::const_iterator const_iterator;

  const_iterator const end = x.end();
  for (const_iterator i = x.begin(); i != end; ++i) {
    QDomDocument doc;
    doc.setContent(*i);
    QScopedPointer<Buteo::SyncProfile> profile(
      new Buteo::SyncProfile(doc.documentElement()));

    for (char const ** s = storage_names; *s != 0; ++s) {
      QString const name (
        profile->subProfileNames(Buteo::Profile::TYPE_STORAGE).first());

      // Check if the storage profile name begins with the current
      // storage name.  If so, append it to the list.
      if (name.indexOf(*s) == 0) {
	m_services.append(ServiceInfo(profile.take(), this));
	break;
      }
    }
  }

  // Sort the ServiceInfo object list.
  std::sort(m_services.begin(), m_services.end());
}
