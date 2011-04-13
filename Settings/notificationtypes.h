/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef NOTIFICATIONTYPES_H
#define NOTIFICATIONTYPES_H

#include <QObject>
#include <QList>
#include <QSettings>
#include <QMap>
#include <QDebug>


class NotificationTypes : public QObject
{
    Q_OBJECT

public:
   explicit NotificationTypes(QObject *parent = 0);

public slots:
    void removeType(QString typeToRemove);
    void addType(QString typeToAdd);
    bool isActive(QString typeToCheck);
    bool maxNotifications();

signals:
    void notificationNumberChanged();

private:

    void findNextOpenIndex();
    QSettings notificationSettings;
    QMap<int,QString> settingsNameMap;
    QMap<QString,int> currentNotificationMap;
    int numberOfNotifications;
    int currentIndex;
};

#endif // NOTIFICATIONTYPES_H
