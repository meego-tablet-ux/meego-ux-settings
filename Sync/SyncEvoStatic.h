#ifndef _SYNC_EVO_STATIC_H_
#define _SYNC_EVO_STATIC_H_

#include <QDBusError>
#include <QDBusPendingCall>
#include <QHash>
#include <QList>
#include <QString>
#include <syncevolution-qt-dbus/dbustypes.h>

#include "QProperty.h"

namespace MeeGo {

  namespace Sync {

    class SyncEvoStatic {
    public:
      /* user-friendly names of storage types are retrieved from syncevo source name strings */
      static QHash<QString, QString> storageTypes();

      /* syncevo source name strings are retrieved from user-friendly names of storage types */
      static QHash<QString, QString> reverseStorageTypes();

      /* User-friendly strings for HTTP status codes - why is there no library for this? */
      static QHash<int, QString> httpStatusCodes();

      /* register D-Bus types - ugly */
      static void init();

      /* Perform an asynchronous D-Bus call, annotating the call watcher with the list of properties */
      static QDBusPendingCallWatcher *dbusCall(QList<QProperty> props, QObject *dst, const char *finishedSlot, const QDBusPendingCall &pendingCall);

      /* Dump a D-Bus error */
      static void reportDBusError(const QString &prefix, const QDBusError &error);
    };
  }
}

#define DBUS_CALL_FUNCTION_NAME(c) \
  ((c)->property("DBusFunctionName").toString())

#endif /* !_SYNC_EVO_STATIC_H_ */
