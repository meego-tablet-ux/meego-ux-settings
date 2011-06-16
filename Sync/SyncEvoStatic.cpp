#include <syncevolution-qt-dbus/dbustypes.h>

#include <QDebug>
#include <QMapIterator>

#include "SyncEvoStatic.h"

/*
 * Hash table containing "recognized" storage types, a.k.a configs.
 * If it ain't in this table, it ain't gonna be in the UI, kapeesh?
 */
QHash<QString, QString>
MeeGo::Sync::SyncEvoStatic::storageTypes()
{
  static QHash<QString, QString> niceConfigNames;

  if (niceConfigNames.isEmpty()) {
    niceConfigNames["source/addressbook"]   = QObject::tr("Contacts");
    niceConfigNames["source/calendar"]      = QObject::tr("Appointments");
    niceConfigNames["source/todo"]          = QObject::tr("Tasks");
    niceConfigNames["source/memo"]          = QObject::tr("Memo");
    niceConfigNames["source/calendar+todo"] = QObject::tr("Appointments & Tasks");
  }

  return niceConfigNames;
}

/*
 * Hash table containing the reverse of "recognized" storage types, a.k.a configs.
 */
QHash<QString, QString>
MeeGo::Sync::SyncEvoStatic::reverseStorageTypes()
{
  static QHash<QString, QString> reverseConfigNames;

  if (reverseConfigNames.isEmpty()) {
    reverseConfigNames[QObject::tr("Contacts")]             = "source/addressbook";
    reverseConfigNames[QObject::tr("Appointments")]         = "source/calendar";
    reverseConfigNames[QObject::tr("Tasks")]                = "source/todo";
    reverseConfigNames[QObject::tr("Memo")]                 = "source/memo";
    reverseConfigNames[QObject::tr("Appointments & Tasks")] = "source/calendar+todo";
  }

  return reverseConfigNames;
}

/*
 * Initialize the type library for the Syncevolution D-Bus service
 */
void
MeeGo::Sync::SyncEvoStatic::init()
{
  static bool needsInit = true;

  if (needsInit) {
    syncevolution_qt_dbus_register_types();
    needsInit = false;
  }
}

QHash<int, QString>
MeeGo::Sync::SyncEvoStatic::httpStatusCodes()
{
  static QHash<int, QString> codes;

  if (codes.isEmpty()) {
    codes[100] = QObject::tr("Continue");
    codes[101] = QObject::tr("Switching Protocols");
    codes[102] = QObject::tr("Processing");
    codes[200] = QObject::tr("OK");
    codes[201] = QObject::tr("Created");
    codes[202] = QObject::tr("Accepted");
    codes[203] = QObject::tr("Non-Authoritative Information");
    codes[204] = QObject::tr("No Content");
    codes[205] = QObject::tr("Reset Content");
    codes[206] = QObject::tr("Partial Content");
    codes[207] = QObject::tr("Multi-Status");
    codes[208] = QObject::tr("Already Reported");
    codes[226] = QObject::tr("IM Used");
    codes[300] = QObject::tr("Multiple Choices");
    codes[301] = QObject::tr("Moved Permanently");
    codes[302] = QObject::tr("Found");
    codes[303] = QObject::tr("See Other");
    codes[304] = QObject::tr("Not Modified");
    codes[305] = QObject::tr("Use Proxy");
    codes[306] = QObject::tr("Reserved");
    codes[307] = QObject::tr("Temporary Redirect");
    codes[400] = QObject::tr("Bad Request");
    codes[401] = QObject::tr("Unauthorized");
    codes[402] = QObject::tr("Payment Required");
    codes[403] = QObject::tr("Forbidden");
    codes[404] = QObject::tr("Not Found");
    codes[405] = QObject::tr("Method Not Allowed");
    codes[406] = QObject::tr("Not Acceptable");
    codes[407] = QObject::tr("Proxy Authentication Required");
    codes[408] = QObject::tr("Request Timeout");
    codes[409] = QObject::tr("Conflict");
    codes[410] = QObject::tr("Gone");
    codes[411] = QObject::tr("Length Required");
    codes[412] = QObject::tr("Precondition Failed");
    codes[413] = QObject::tr("Request Entity Too Large");
    codes[414] = QObject::tr("Request-URI Too Long");
    codes[415] = QObject::tr("Unsupported Media Type");
    codes[416] = QObject::tr("Requested Range Not Satisfiable");
    codes[417] = QObject::tr("Expectation Failed");
    codes[422] = QObject::tr("Unprocessable Entity");
    codes[423] = QObject::tr("Locked");
    codes[424] = QObject::tr("Failed Dependency");
    codes[425] = QObject::tr("Reserved for WebDAV advanced");
    codes[426] = QObject::tr("Upgrade Required");
    codes[500] = QObject::tr("Internal Server Error");
    codes[501] = QObject::tr("Not Implemented");
    codes[502] = QObject::tr("Bad Gateway");
    codes[503] = QObject::tr("Service Unavailable");
    codes[504] = QObject::tr("Gateway Timeout");
    codes[505] = QObject::tr("HTTP Version Not Supported");
    codes[506] = QObject::tr("Variant Also Negotiates");
    codes[507] = QObject::tr("Insufficient Storage");
    codes[508] = QObject::tr("Loop Detected");
    codes[509] = QObject::tr("Unassigned");
    codes[510] = QObject::tr("Not Extended");
  }

  return codes;
}

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
  QString msg =
    prefix.isEmpty()
      ? QString("")
      : QString(prefix + ": ")
    + QDBusError::errorString(error.type())
    + QString(": ")
    + error.name()
    + QString(": ")
    + error.message();

  qDebug() << msg;
}
