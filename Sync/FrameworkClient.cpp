/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include "FrameworkClient.hpp"
#include "SharedCredentials.hpp"
#include "StoreCredentialsProcessor.hpp"
#include "RemoveCredentialsProcessor.hpp"

#include <SyncProfile.h>
#include <ProfileManager.h>
#include <SyncResults.h>
// #include <QDomDocument>
#include <QDebug>

#include <MNotification>


namespace {
  // Keep these around for future use so that translators need not
  // translate them later.

  //: Suggested action for transient issue, e.g. connection failure.
  char const TRY_AGAIN[] = "please try again later";

  //: Problem with sync database.  Arg 1 is storage type (contacts, etc).
  char const DATABASE_ERROR[] = "sorry %1 database error";

  //: Unknown/unexpected problem occurred during sync.
  char const OTHER_ERROR[] = "something went wrong";
}

MeeGo::Sync::FrameworkClient::FrameworkClient(QObject* parent)
  : QObject(parent)
  , m_sci()
  , m_pm()
  , m_processor()
  , m_cred()
  , m_scheduled(false)
  , m_status()
  , m_service()
  , m_storage()
  , m_name()
  , m_username()
  , m_password()
  , m_provider()
  , m_removalsPending(0)
{
  // --- Sync related events ---
  connect(&m_sci,
	  SIGNAL(syncStatus(QString,int,QString,int)),
	  this,
	  SLOT(syncStatus(QString,int,QString,int)));

  connect(&m_sci,
	  SIGNAL(resultsAvailable(QString,Buteo::SyncResults)),
	  this,
	  SLOT(resultsAvailable(QString,Buteo::SyncResults)));

  connect(&m_sci,
	  SIGNAL(profileChanged(QString,int,QString)),
	  this,
	  SLOT(profileChanged(QString,int,QString)));
}

MeeGo::Sync::FrameworkClient::~FrameworkClient()
{
}

bool
MeeGo::Sync::FrameworkClient::scheduled() const
{
  return m_scheduled;
}

void
MeeGo::Sync::FrameworkClient::setScheduled(bool s)
{
  // Be careful not to emit the scheduledChanged() signal if the
  // "scheduled" value has not changed.

  if (s != m_scheduled) {
    m_scheduled = s;
    emit scheduledChanged(s);
  }
}

QString
MeeGo::Sync::FrameworkClient::status() const
{
  return m_status;
}

void
MeeGo::Sync::FrameworkClient::setStatus(QString s)
{
  // Be careful not to emit the statusChanged() signal if the value of
  // the status string has not changed.

  if (s != m_status) {
    m_status = s;
    emit statusChanged(s);
  }
}

QString
MeeGo::Sync::FrameworkClient::service() const
{
  return m_service;
}

void
MeeGo::Sync::FrameworkClient::setService(QString s)
{
  if (s != m_service) {
    m_service = s;

    // @note Service name, *not* profile name/ID, is passed to the
    //       getSsoCaption() function!
    m_provider = getSsoCaption(s);
  }
}

QString
MeeGo::Sync::FrameworkClient::storage() const
{
  return m_storage;
}

void
MeeGo::Sync::FrameworkClient::setStorage(QString s)
{
  if (s != m_storage) {
    m_storage = s;
  }
}

QString
MeeGo::Sync::FrameworkClient::name() const
{
  return m_name;
}

void
MeeGo::Sync::FrameworkClient::setName(QString s)
{
  if (s != m_name) {
    m_name = s;
  }
}

QString
MeeGo::Sync::FrameworkClient::username() const
{
  return m_username;
}

void
MeeGo::Sync::FrameworkClient::setUsername(QString s)
{
  if (s != m_username) {
    m_username = s;
  }
}

QString
MeeGo::Sync::FrameworkClient::password() const
{
  return m_password;
}

void
MeeGo::Sync::FrameworkClient::setPassword(QString s)
{
  if (s != m_password) {
    m_password = s;
  }
}

QDateTime
MeeGo::Sync::FrameworkClient::lastSyncTime()
{
  Buteo::SyncResults const results = m_sci.getLastSyncResult(m_name);

  QDateTime const t (results.syncTime());
  qDebug() << "INFO: Last sync time:" << t.toString();

  return t;
}

QString
MeeGo::Sync::FrameworkClient::getSsoCaption(QString const & name)
{
  if (name.contains("google", Qt::CaseInsensitive)
      || name.contains("gmail", Qt::CaseInsensitive))
    return "google.com";
  else if (name.contains("yahoo", Qt::CaseInsensitive))
    return "yahoo.com";
  else
    return name;
}

QString
MeeGo::Sync::FrameworkClient::syncResultToString(
  Buteo::SyncResults const & results)
{
  QString e;
  int const major = results.majorCode();

  if (major == Buteo::SyncResults::SYNC_RESULT_SUCCESS) {
    //: Status message: successfully completed sync job. (PLACEHOLDER)
    e = tr("completed");

  } else if (major == Buteo::SyncResults::SYNC_RESULT_FAILED) {

    switch(results.minorCode()) {
    case Buteo::SyncResults::SYNC_FINISHED:
      //: Sync finished with potential(?) error.
      e = tr("finished");  // Is this really an error?
      break;

    case Buteo::SyncResults::INTERNAL_ERROR:
      //: Internal error occurred during sync.
      e = tr("internal error");
      break;

    case Buteo::SyncResults::AUTHENTICATION_FAILURE:
      //: Authentication failure occurred during sync.
      e = tr("authentication failure");
      break;

      /// @todo INFORM USER: SORRY <STORAGE> DATABASE ERROR
    case Buteo::SyncResults::DATABASE_FAILURE:
      //: Database access error occurred during sync.
      e = tr("database failure");
      break;

    case Buteo::SyncResults::SUSPENDED:
      //: Sync job suspended.
      e = tr("suspended");
      break;

    case Buteo::SyncResults::ABORTED:
      //: Sync job aborted.
      e = tr("aborted");
      break;

      // @todo INFORM USER TO PLEASE TRY AGAIN
    case Buteo::SyncResults::CONNECTION_ERROR:
      //: Connection to remote sync service failed during sync.
      e = tr("connection error");
      break;

      // INFORM USER TO PLEASE TRY AGAIN
    case Buteo::SyncResults::INVALID_SYNCML_MESSAGE:
      //: Invalid syncml message/request submitted during sync.
      e = tr("invalid syncml message");
      break;

      // LOG THE FOLLOWING - DO NOT SHOW USER
    case Buteo::SyncResults::UNSUPPORTED_SYNC_TYPE:
      //: Unsupported sync type (manual or auto?)
      e = tr("unsupported sync type");
      break;

      // LOG THE FOLLOWING - DO NOT SHOW USER
    case Buteo::SyncResults::UNSUPPORTED_STORAGE_TYPE:
      //: "Storage" refers to contacts, calendar, etc.
      e = tr("unsupported storage type");
      break;

      // INFORM USER: SOMETHING WENT WRONG
    default:
      // One case where error appears to is when a required backend
      // (e.g. for calendar) is not available to msyncd.

      //: Unknown error occurred during sync.
      e = tr("other");
      break;
    }
  }

  return e;
}

void
MeeGo::Sync::FrameworkClient::doPostInit(QString fuzzyTime,
					 bool forceSync)
{
  if (m_name.isEmpty()) {
    qCritical() << "ERROR: Empty profile name being used!";
    emit profileRemoved(m_name);  // Force return to main screen.
    return;
  }

  // Get the last sync result and update the status message to contain
  // the last time a sync occurred, or if it failed.  If no sync
  // result exists a sync has never been run so perform an initial
  // sync.

  Buteo::SyncResults const results = m_sci.getLastSyncResult(m_name);

  if (results.majorCode() == Buteo::SyncResults::SYNC_RESULT_INVALID
      || forceSync) {

    doInitialSync();  // Never been "synced".

  } else {

    // Display the status of the last sync.
    if (results.majorCode() == Buteo::SyncResults::SYNC_RESULT_SUCCESS) {
      //: Arg 1 is a "fuzzy time", e.g. "2 min ago".
      setStatus(tr("Last sync %1").arg(fuzzyTime));
    } else {
      // Last sync failed.  Display how it failed.
      QString const e = syncResultToString(results);

      //: Arg 1 is a brief sync error description, e.g. "connection failed".
      setStatus(tr("Last sync failed: %1").arg(e));

      // We could pop up the login dialog on authentication failure of
      // the last sync by emitting the authentication_failed()
      // signal.  However, doing so could inadvertently make the user
      // re-enter shared credentials that were previously updated for
      // another account that uses those same credentials.  For
      // example, the user could have updated the credentials for
      // Google Calendar because of an authentication failure.  We
      // would not to immediately pop-up the login dialog if the last
      // sync for Google Contacts had an authentication failure.  We'd
      // want to use the updated shared credentials.  As such, we only
      // pop up the login dialog *immediately* after an authentication
      // failure.
    }

    // Flip the recurring sync toggle as necessary.
    QScopedPointer<Buteo::SyncProfile> profile(m_pm.syncProfile(m_name));

    if (profile->syncType() == Buteo::SyncProfile::SYNC_SCHEDULED)
      setScheduled(true);
  }
}

void
MeeGo::Sync::FrameworkClient::doInitialSync()
{
  qDebug() << "INFO: Performing 'initial sync' procedure for profile:"
	   << m_name;

  StoreCredentialsProcessor * const processor =
    new StoreCredentialsProcessor(m_username, m_password);
  m_processor.reset(processor);

  // Capture completion of credentials storage for subsequent sync
  // profile processing.
  connect(processor,
	  SIGNAL(stored(quint32)),
	  this,
	  SLOT(credentialsStored(quint32)));

  // Capture errors that occurred during credentials storage.
  connect(processor,
	  SIGNAL(error(const SignOn::Error &)),
	  this,
	  SLOT(credentialsError(const SignOn::Error &)));

  if (m_cred.isNull()) {
    m_cred.reset(new SharedCredentials);
  }

  // Process credentials storage asynchronously.
  m_cred->processCredentials(processor, m_provider);

  // Profile will be updated with new username/password once
  // credentials storage completes.
}

void
MeeGo::Sync::FrameworkClient::syncNow()
{
  bool const profile_found = m_sci.startSync(m_name);

  if (!profile_found) {
    // Internal error!
    qWarning() << "WARNING: profile " << m_name << "not found";
  }
}

void
MeeGo::Sync::FrameworkClient::enableAutoSync(bool enable)
{
  QScopedPointer<Buteo::SyncProfile> profile(m_pm.syncProfile(m_name));

  Buteo::SyncProfile::SyncType const s =
    enable
    ? Buteo::SyncProfile::SYNC_SCHEDULED
    : Buteo::SyncProfile::SYNC_MANUAL;

  if (s != profile->syncType()) {
    // Toggle the SyncType to enable or disable recurring syncs.

    profile->setSyncType(s);

    if(!m_sci.updateProfile(*profile))
      qWarning() << "WARNING: problem toggling recurring sync";

    setScheduled(enable);
  }
}

void
MeeGo::Sync::FrameworkClient::forgetProfile()
{
  // Remove the corresponding credentials before removing the profiles
  // so that the credentials are not available in the profileChanged()
  // slot in the StorageModel.

  RemoveCredentialsProcessor * const processor =
    new RemoveCredentialsProcessor;
  m_processor.reset(processor);

  // Capture completion of credentials removal for subsequent sync
  // profile processing.
  connect(processor,
	  SIGNAL(removed()),
	  this,
	  SLOT(credentialsRemoved()));

  // Capture errors that occurred during credentials removal.
  connect(processor,
	  SIGNAL(error(const SignOn::Error &)),
	  this,
	  SLOT(credentialsError(const SignOn::Error &)));

  if (m_cred.isNull()) {
    m_cred.reset(new SharedCredentials);
  }

  m_cred->processCredentials(processor, m_provider);

  // Profile will be removed once credentials removal completes.
}

void
MeeGo::Sync::FrameworkClient::syncStatus(QString id,
					 int statusCode,
					 QString /* message */,
					 int /* details */)
{
  QString s;

  switch(statusCode) {
  case 0:   // QUEUED
    qDebug() << "INFO: sync request queued for" << id;
    //: Status message: sync request/job has been queued
    s = tr("Sync request queued");
    break;

  case 1:   // STARTED
    qDebug() << "INFO: sync started for" << id;
    //: Status message: sync job started
    s = tr("Sync started");
    break;

  case 2:   // PROGRESS
    qDebug() << "INFO: syncing" << id << "now ...";
    //: Status message: Sync in-progress
    s = tr("Syncing now...");
    break;

  case 3:   // ERROR
    qDebug() << "INFO: sync failed for" << id;
    // Don't set the status message here.  The resultsAvailable() slot
    // provides more detailed error messages.
    return;

  case 4:   // DONE
    qDebug() << "INFO: sync completed for" << id;
    //: Status message: Sync job completed
    s = tr("Sync completed");
    break;

  case 5:   // ABORTED
    qDebug() << "INFO: sync aborted for" << id;
    //: Sync explicitly stopped.
    s = tr("Sync aborted");
    break;

  default:
    // Yikes!  We should never get here!
    qWarning() << "WARNING: unknown sync status for" << id;
    //: Status message: Displayed when an unknown/unhandled error occurs.
    s = tr("Unknown sync status");
    break;
  }

  setStatus(s);
}

void
MeeGo::Sync::FrameworkClient::resultsAvailable(
  QString id,
  Buteo::SyncResults results)
{
  QString s;
  int const major = results.majorCode();

  if (major == Buteo::SyncResults::SYNC_RESULT_SUCCESS) {
    // @todo Should we instead display a time?
    //: Status message: successfully completed sync job.
    s = tr("Sync completed");

  } else if (major == Buteo::SyncResults::SYNC_RESULT_FAILED) {
    QString const e = syncResultToString(results);

    qWarning() << "WARNING:" << id << "sync failure:" << e;

    //: Sync result message.  The argument provides the error details.
    s = tr("Sync failed: %1").arg(e);

    // Issue a soft notification about the failure.
    MNotification n("Sync");
    n.setSummary(tr("%1 %2 sync failed").arg(m_service).arg(m_storage));
    n.setBody(e);
    n.setImage("image://meegotheme/icons/settings/sync");
    n.publish();

    // Pop up the login dialog on authentication failure.
    if (results.minorCode() == Buteo::SyncResults::AUTHENTICATION_FAILURE) {
      emit authenticationFailed();
    }
  }

  setStatus(s);
}

void
MeeGo::Sync::FrameworkClient::profileChanged(QString id,
					     int type,
					     QString /* profile_xml */)
{
  // Used predominantly for debugging.

  switch(type) {
  case 0:  // added
    qDebug() << "INFO: profile" << id << "added.";
    break;

  case 1:  // modified
//     {
       qDebug() << "INFO: profile" << id << "modified.";
//       QDomDocument doc;
//       doc.setContent(profile_xml);
//       Buteo::SyncProfile profile(doc.documentElement());
//       setScheduled(profile.syncType() == Buteo::SyncProfile::SYNC_SCHEDULED);
//     }

    break;

  case 2:  // deleted
    qDebug() << "INFO: profile" << id << "removed.";

     if (--m_removalsPending == 0) {
       // All profiles with same SSO provider have been removed.
       emit profileRemoved(id);  // Force return to main screen.
     }

    break;

  default:
    qWarning() << "WARNING: Unknown type ("
	       << type << ") of profile change occured.";
    break;
  }
}

void
MeeGo::Sync::FrameworkClient::credentialsStored(quint32)
{
  // Credentials have been stored.  Update the sync profile
  // accordingly.

  m_processor.reset();

  QScopedPointer<Buteo::SyncProfile> profile(m_pm.syncProfile(m_name));

  profile->setKey("Username", "sso-provider=" + m_provider);

  // Either create a copy of the sync profile in the system directory
  // (e.g. /etc/sync/...") that will ultimately be stored in the
  // user's home directory (~/.sync/...), or update one that already
  // exists in the user's home directory.

  // @todo Can we create a top-level sync profile for Google that
  //       contains both hcalendar and hcontacts storage sub-profiles?
  //       Currently they are separate top-level sync profiles in the
  //       system sync profile directory.

  // Save changes to disk.
  if (!m_sci.updateProfile(*profile)) {
    //: Failed to update and store sync account/profile info on disk.
    setStatus(tr("Unable to update sync profile"));
  }

  qDebug() << "Username: " << m_username;
  // qDebug() << "Password: " << m_password;
  qDebug() << "SSO Username: " << ("sso-provider=" + m_provider);

  // Enable recurring sync.
  //
  // The default sync interval is 60 minutes.
  //
  // @note Every hour was an arbitrary choice that seemed
  //       reasonable.
  //
  // @todo The default interval should be configurable through a
  //       configuration file (e.g. in /etc) so that vendors can
  //       easily choose their own.

  unsigned const MINUTES = 60;
  Buteo::SyncSchedule schedule;
  schedule.setInterval(MINUTES);

  qDebug() << "INFO: Setting sync schedule to:" << schedule.toString();

  // Set the schedule.
  if (m_sci.setSyncSchedule(m_name, schedule)) {
    setScheduled(true);
    syncNow();
  } else {
    //: Attempt to set automatic sync schedule failed.
    setStatus(tr("Sync scheduling failed"));
  }
}

void
MeeGo::Sync::FrameworkClient::credentialsRemoved()
{
  // Credentials have been removed.  Now remove the corresponding sync
  // profiles.

  m_processor.reset();

  // Remove all profiles with matching SSO provider.
  Buteo::ProfileManager::SearchCriteria criterion;
  criterion.iType = Buteo::ProfileManager::SearchCriteria::EQUAL;
  criterion.iKey = "Username";
  criterion.iValue = "sso-provider=" + m_provider;

  QList<Buteo::ProfileManager::SearchCriteria> criteria;
  criteria.append(criterion);

  typedef QList<Buteo::SyncProfile*> list_type;
  list_type profiles = m_pm.getSyncProfilesByData(criteria);

  int const count = profiles.size();

  qDebug() << "INFO:" << count << "sync profiles will be removed.";

  m_removalsPending = count;

  list_type::const_iterator const end = profiles.end();
  for (list_type::const_iterator i = profiles.begin(); i != end; ++i) {
    // Buteo interface is busted.  It expects reference to non-const
    // QString, so we can't pass temporaries to the removeProfile()
    // method.  Explicitly instantiate a QString on the stack.  *sigh*
    QString hack((*i)->name());

    qDebug() << "INFO: Removing profile" << hack;

    // Remove profile from disk
    if (!m_sci.removeProfile(hack)) {
      //: Displayed when removal of sync account information fails.
      setStatus(tr("Unable to forget sync account!"));
    }
  }

  qDeleteAll(profiles);
  profiles.clear();
}

void
MeeGo::Sync::FrameworkClient::credentialsError(const SignOn::Error & e)
{
  m_processor.reset();

  switch (e.type()) {
  case SignOn::Error::StoreFailed:
    //: Internal error.  Failed to store user supplied credentials in DB.
    setStatus (tr("Failed to store credentials"));
    break;
  case SignOn::Error::RemoveFailed:
    //: Displayed when clearing stored sync account credentials fails.
    setStatus (tr("Failed to clear credentials"));
    break;
  default:
    //: We're really not expecting any other kinds of cred errors.
    setStatus(tr("Unknown credentials error"));
    break;
  }
}
