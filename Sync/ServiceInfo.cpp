/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include "ServiceInfo.hpp"

#include <SyncProfile.h>
#include <QDebug>


MeeGo::Sync::ServiceInfo::ServiceInfo(Buteo::SyncProfile * profile,
				      QObject * parent)
  : QObject(parent)
  , m_profile(profile)
  , m_storage(profile->subProfileNames(Buteo::Profile::TYPE_STORAGE).first())
{
}

MeeGo::Sync::ServiceInfo::ServiceInfo(ServiceInfo const & rhs)
  : QObject(rhs.parent())
  , m_profile(new Buteo::SyncProfile(*rhs.m_profile))
  , m_storage(rhs.m_storage)
{
}

void MeeGo::Sync::ServiceInfo::operator=(ServiceInfo const & rhs)
{
  // Weak exception-safety.

  // @todo It would be nice if we implement strong exception safety
  //       and remove this self-assignment check by using the
  //       copy-and-swap idiom.
  if (this == &rhs)
    return;

  m_profile.reset(new Buteo::SyncProfile(*rhs.m_profile));

  m_storage  = rhs.m_storage;

  setParent(rhs.parent());
}

MeeGo::Sync::ServiceInfo::~ServiceInfo()
{
}

QString MeeGo::Sync::ServiceInfo::displayName() const
{
  return m_profile->displayname();
}

QString MeeGo::Sync::ServiceInfo::profileName() const
{
  return m_profile->name();
}

QString MeeGo::Sync::ServiceInfo::imageFilename() const
{
  QString const name(m_profile->displayname());

  // @todo Do not hardcode.  Pull icon name from an XML file.
  if (name == "Google")
    return "image://themedimage/icons/services/gmail";
  else if (name == "Yahoo!")
    return "image://themedimage/icons/services/yahoo"; // Do we need a sync version?
  else if (name == "Mobical")
    return "image://themedimage/icons/services/mobical";

  return QString();
}

bool MeeGo::Sync::ServiceInfo::active() const
{
  return m_profile->syncType() == Buteo::SyncProfile::SYNC_SCHEDULED;
}

QString MeeGo::Sync::ServiceInfo::storage() const
{
  // This approach is used since some storage profiles may have
  // additional characters appended to the "well known" storage names,
  // e.g. "hcontacts-google" instead of "hcontacts".

  if (m_storage.indexOf("hcalendar") == 0) {

    //: Sync storage type often used like so: "Sync Calendar" or
    //"Google Calendar".
    return tr("Calendar");

  } else if (m_storage.indexOf("hcontacts") == 0) {

    //: Sync storage type often used like so: "Sync Contacts" or
    //: "Google Contacts".
    return tr("Contacts");

  }

  qWarning() << "WARNING: Unknown sync storage type:" << m_storage;

  return QString();
}

bool
MeeGo::Sync::ServiceInfo::operator<(ServiceInfo const & rhs) const
{
  // Lexicographically compare on storage type than service display
  // name.
  return
    m_storage < rhs.m_storage
    || (m_storage == rhs.m_storage && displayName() < rhs.displayName());
}
