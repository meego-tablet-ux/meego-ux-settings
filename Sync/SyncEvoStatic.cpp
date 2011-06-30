#include <syncevolution-qt-dbus/dbustypes.h>

#include <QDebug>
#include <QMapIterator>

#include "SyncEvoStatic.h"

MeeGo::Sync::SyncEvoStatic::SyncEvoStatic(QObject *parent)
  : QObject(parent)
{
  syncevolution_qt_dbus_register_types();

  //: Sync storage type often used like so: "Sync Contacts" or
  //: "Google Contacts".
  m_niceConfigNames["source/addressbook"]   = tr("Contacts");

  //: Sync storage type often used like so: "Sync Calendar" or
  //"Google Calendar".
  m_niceConfigNames["source/calendar"]      = tr("Calendar");
/*
 * FIXME: These need new strings
 */
#if (0)
  m_niceConfigNames["source/todo"]          = tr("Tasks");
  m_niceConfigNames["source/memo"]          = tr("Memo");
  m_niceConfigNames["source/calendar+todo"] = tr("Calendar & Tasks");

  m_httpStatusCodes[100] = tr("Continue");
  m_httpStatusCodes[101] = tr("Switching Protocols");
  m_httpStatusCodes[102] = tr("Processing");
  m_httpStatusCodes[200] = tr("OK");
  m_httpStatusCodes[201] = tr("Created");
  m_httpStatusCodes[202] = tr("Accepted");
  m_httpStatusCodes[203] = tr("Non-Authoritative Information");
  m_httpStatusCodes[204] = tr("No Content");
  m_httpStatusCodes[205] = tr("Reset Content");
  m_httpStatusCodes[206] = tr("Partial Content");
  m_httpStatusCodes[207] = tr("Multi-Status");
  m_httpStatusCodes[208] = tr("Already Reported");
  m_httpStatusCodes[226] = tr("IM Used");
  m_httpStatusCodes[300] = tr("Multiple Choices");
  m_httpStatusCodes[301] = tr("Moved Permanently");
  m_httpStatusCodes[302] = tr("Found");
  m_httpStatusCodes[303] = tr("See Other");
  m_httpStatusCodes[304] = tr("Not Modified");
  m_httpStatusCodes[305] = tr("Use Proxy");
  m_httpStatusCodes[306] = tr("Reserved");
  m_httpStatusCodes[307] = tr("Temporary Redirect");
  m_httpStatusCodes[400] = tr("Bad Request");
  m_httpStatusCodes[401] = tr("Unauthorized");
  m_httpStatusCodes[402] = tr("Payment Required");
  m_httpStatusCodes[403] = tr("Forbidden");
  m_httpStatusCodes[404] = tr("Not Found");
  m_httpStatusCodes[405] = tr("Method Not Allowed");
  m_httpStatusCodes[406] = tr("Not Acceptable");
  m_httpStatusCodes[407] = tr("Proxy Authentication Required");
  m_httpStatusCodes[408] = tr("Request Timeout");
  m_httpStatusCodes[409] = tr("Conflict");
  m_httpStatusCodes[410] = tr("Gone");
  m_httpStatusCodes[411] = tr("Length Required");
  m_httpStatusCodes[412] = tr("Precondition Failed");
  m_httpStatusCodes[413] = tr("Request Entity Too Large");
  m_httpStatusCodes[414] = tr("Request-URI Too Long");
  m_httpStatusCodes[415] = tr("Unsupported Media Type");
  m_httpStatusCodes[416] = tr("Requested Range Not Satisfiable");
  m_httpStatusCodes[417] = tr("Expectation Failed");
  m_httpStatusCodes[422] = tr("Unprocessable Entity");
  m_httpStatusCodes[423] = tr("Locked");
  m_httpStatusCodes[424] = tr("Failed Dependency");
  m_httpStatusCodes[425] = tr("Reserved for WebDAV advanced");
  m_httpStatusCodes[426] = tr("Upgrade Required");
  m_httpStatusCodes[500] = tr("Internal Server Error");
  m_httpStatusCodes[501] = tr("Not Implemented");
  m_httpStatusCodes[502] = tr("Bad Gateway");
  m_httpStatusCodes[503] = tr("Service Unavailable");
  m_httpStatusCodes[504] = tr("Gateway Timeout");
  m_httpStatusCodes[505] = tr("HTTP Version Not Supported");
  m_httpStatusCodes[506] = tr("Variant Also Negotiates");
  m_httpStatusCodes[507] = tr("Insufficient Storage");
  m_httpStatusCodes[508] = tr("Loop Detected");
  m_httpStatusCodes[509] = tr("Unassigned");
  m_httpStatusCodes[510] = tr("Not Extended");
#endif /* (0) */
}

MeeGo::Sync::SyncEvoStatic::~SyncEvoStatic()
{
}

MeeGo::Sync::SyncEvoStatic *
MeeGo::Sync::SyncEvoStatic::instance()
{
  static MeeGo::Sync::SyncEvoStatic *singleton = 0;

  if (0 == singleton)
    singleton = new MeeGo::Sync::SyncEvoStatic;

  return singleton;
}

/*
 * Hash table containing "recognized" storage types, a.k.a configs.
 * If it ain't in this table, it ain't gonna be in the UI, kapeesh?
 */
QHash<QString, QString>
MeeGo::Sync::SyncEvoStatic::storageTypes() const
{
  return m_niceConfigNames;
}

/*
 * FIXME: Needs new strings
 */
#if (0)
QHash<int, QString>
MeeGo::Sync::SyncEvoStatic::httpStatusCodes() const
{
  return m_httpStatusCodes;
}
#endif /* (0) */
/*
 * Async D-Bus call with properties.
 *
 * Compose this function with the actual D-Bus call, whose return value goes into the last argument.
 *
 * Creates a QDBusPendingCallWatcher from @pendingCall, annotating it with the properties stored in @props, and connects
 * the completion signal. Thus, the completion signal handler, which will receive the call watcher as a parameter, can
 * have more information about what call has completed than it would get from the call watcher alone (which is no info at
 * all).
 */
QDBusPendingCallWatcher *
MeeGo::Sync::SyncEvoStatic::dbusCall(QList<QProperty> props, QObject *dst, const char *finishedSlot,
  const QDBusPendingCall &pendingCall)
{
  QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(pendingCall, dst);

  for (int Nix = 0 ; Nix < props.count() ; Nix++)
    watcher->setProperty(props[Nix].toUtf8().constData(), props[Nix].value());

  QObject::connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher *)), dst, finishedSlot);

  return watcher;
}

/*
 * Report a D-Bus error
 */
void
MeeGo::Sync::SyncEvoStatic::reportDBusError(const QString &prefix, const QDBusError &error)
{
  qDebug() <<
    (prefix.isEmpty()
      ? QString("")
      : QString(prefix + ": "))
    + QDBusError::errorString(error.type())
    + QString(": ") + error.name()
    + QString(": ") + error.message();
}
