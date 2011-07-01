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
