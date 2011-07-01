#ifndef _SYNC_EVO_STATIC_H_
#define _SYNC_EVO_STATIC_H_

#include <QDBusError>
#include <QDBusPendingCall>
#include <QHash>
#include <QList>
#include <QObject>
#include <QString>
#include <syncevolution-qt-dbus/dbustypes.h>

#include "QProperty.h"

namespace MeeGo {

  namespace Sync {

    class SyncEvoStatic : public QObject {
      Q_OBJECT

    public:
      static SyncEvoStatic *instance();

      SyncEvoStatic(QObject *parent = 0);
      virtual ~SyncEvoStatic();

      /* user-friendly names of storage types are retrieved from syncevo source name strings */
      QHash<QString, QString> storageTypes() const;
#if (0)
      /* User-friendly strings for HTTP status codes - why is there no library for this? */
      QHash<int, QString> httpStatusCodes() const;
#endif /* (0) */

      /* Perform an asynchronous D-Bus call, annotating the call watcher with the list of properties */
      static QDBusPendingCallWatcher *dbusCall(QList<QProperty> props, QObject *dst, const char *finishedSlot, const QDBusPendingCall &pendingCall);

      /* Dump a D-Bus error */
      static void reportDBusError(const QString &prefix, const QDBusError &error);

    private:
      QHash<QString, QString> m_niceConfigNames;
#if (0)
      QHash<QString, QString> m_httpStatusCodes;
#endif /* (0) */
    };
  }
}

#define DBUS_CALL_FUNCTION_NAME(c) \
  ((c)->property("DBusFunctionName").toString())

#endif /* !_SYNC_EVO_STATIC_H_ */
