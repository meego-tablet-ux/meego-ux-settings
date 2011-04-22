/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include "ServiceInfo.hpp"
#include "SharedCredentials.hpp"
#include "RetrieveCredentialsProcessor.hpp"
#include "StorageModel.hpp"

#include <SyncProfile.h>
#include <SignOn/Error>
#include <QDebug>


MeeGo::Sync::ServiceInfo::ServiceInfo(Buteo::SyncProfile * profile,
				      StorageModel * model,
				      QObject * parent)
  : QObject(parent)
  , m_profile(profile)
  , m_model(model)
  , m_processor()
  , m_cred()
  , m_storage(profile->subProfileNames(Buteo::Profile::TYPE_STORAGE).first())
  , m_username(syncValue("Username"))
  , m_password(syncValue("Password"))
  , m_ready(m_username.indexOf("sso-provider=") != 0) // No async retrieval
{
  connect(this,
	  SIGNAL(serviceReady(QString)),
	  m_model,
	  SLOT(serviceReady(QString)));
}

MeeGo::Sync::ServiceInfo::ServiceInfo(ServiceInfo const & rhs)
  : QObject(rhs.parent())
  , m_profile(new Buteo::SyncProfile(*rhs.m_profile))
  , m_model(rhs.m_model)   // No need to deep copy.
  , m_processor() // Re-instantiated as needed.
  , m_cred()      // Re-instantiated as needed.
  , m_storage(rhs.m_storage)
  , m_username(rhs.m_username)
  , m_password(rhs.m_password)
  , m_ready(rhs.m_ready)
{
  connect(this,
	  SIGNAL(serviceReady(QString)),
	  m_model,
	  SLOT(serviceReady(QString)));
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

  // No need to deep copy since m_model owns us.
  m_model = rhs.m_model;

  // Re-instantiated as needed.
  m_processor.reset();
  m_cred.reset();

  m_storage  = rhs.m_storage;
  m_username = rhs.m_username;
  m_password = rhs.m_password;
  m_ready    = rhs.m_ready;

  disconnect(SIGNAL(serviceReady(QString)));

  setParent(rhs.parent());

  connect(this,
	  SIGNAL(serviceReady(QString)),
	  m_model,
	  SLOT(serviceReady(QString)));
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
    return "image://meegotheme/icons/services/gmail";
  else if (name == "Yahoo!")
    return "image://meegotheme/icons/services/yahoo"; // Do we need a sync version?
  else if (name == "Mobical")
    return "image://meegotheme/icons/services/mobical";

  return QString();
}

bool MeeGo::Sync::ServiceInfo::active() const
{
  return m_profile->syncType() == Buteo::SyncProfile::SYNC_SCHEDULED;
}

QString MeeGo::Sync::ServiceInfo::username() const
{
  // Check if we need to retrieve credentials from SSO.
  if (!m_ready || m_username.isEmpty())
    retrieveCredentials();  // Asynchronous

  return m_username;
}

QString MeeGo::Sync::ServiceInfo::password() const
{
  // We never retrieve the password alone.  Even if the password isn't
  // set it will eventually be retrieved along with the username.  No
  // need to initiate independent credentials retrieval.

  return m_password;
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
MeeGo::Sync::ServiceInfo::ready() const
{
  return m_ready;
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

void
MeeGo::Sync::ServiceInfo::credentials(const QString & username,
				      const QString & password)
{
  m_username = username;
  m_password = password;

  m_processor.reset();

  setReady();
}

void
MeeGo::Sync::ServiceInfo::error(const SignOn::Error & e)
{

  qWarning()
    << "WARNING: Unable to retrieve username from SSO framework -"
    << e.message();

  m_processor.reset();

  // Give user a chance to enter new credentials.
  setReady();
}

QString
MeeGo::Sync::ServiceInfo::syncValue(QString const & key)
{
  QStringList const values = m_profile->keyValues(key);

  int const sz = values.size();

  if (sz > 1) {
    // Sanity check.  We're only using the first value, since we're
    // only expecting to retrieve values with 1-to-1 mappings to
    // their  // corresponding key.
    qWarning() << "Multiple values detected for sync profile key\""
	       << key << "in sync profile \"" << profileName()
	       << "\".  Only the first will be used.";
  } else if (sz < 1) {
    // No value set yet.
    return QString();
  }

  return values[0];
}

void
MeeGo::Sync::ServiceInfo::setReady()
{
  // Be careful not to emit the serviceReady() signal if the 'ready'
  // status is already true.

//   if (!m_ready) {
    m_ready = true;
    emit serviceReady(profileName());
//   }
}

void
MeeGo::Sync::ServiceInfo::retrieveCredentials() const
{
  if (!m_processor.isNull()) {
    return;  // Credentials retrieval in progress.
  }

  QString username(m_username);

  // No username is set, so check if we already have suitable shared
  // credentials stored in the SSO framework.
  if (username.isEmpty() || username.isNull()) {
    QString const n = profileName();

    // @todo Refactor the common code found here and in
    //       FrameworkClient.cpp.
    if (n.contains("google", Qt::CaseInsensitive)
	|| n.contains("gmail", Qt::CaseInsensitive)) {
      username = "sso-provider=google.com";
    } else if (n.contains("yahoo", Qt::CaseInsensitive)) {
      username = "sso-provider=yahoo.com";
    }
  }

  qDebug() << "Retrieving credentials for" << username;

  // If we have an SSO credential, retrieve the username from the
  // SSO framework.
  if (username.contains("sso-provider=")) {
    // Caption is the field to the right of the equal sign.
    QString const caption = username.section('=', 1, 1);

    m_processor.reset(new RetrieveCredentialsProcessor);

    // Capture completion of credentials retrieval for subsequent sync
    // profile processing.
    connect(m_processor.data(),
	    SIGNAL(credentials(const QString &, const QString &)),
	    this,
	    SLOT(credentials(const QString &, const QString &)));

    // Capture errors that occurred during credentials retrieval.
    connect(m_processor.data(),
	    SIGNAL(error(const SignOn::Error &)),
	    this,
	    SLOT(error(const SignOn::Error &)));

    if (m_cred.isNull()) {
      m_cred.reset(new SharedCredentials);
    }

    m_cred->processCredentials(m_processor.data(), caption);
  }
}
