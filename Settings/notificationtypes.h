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
#include <QMap>
#include <MGConfItem>
#include <QDebug>

#define MAXNOTIFICATIONS 4

class NotificationTypes : public QObject
{
    Q_OBJECT

public:
   explicit NotificationTypes(QObject *parent = 0);

public slots:
    void removeType(QVariant typeToRemove);
    void addType(QVariant typeToAdd);
    bool isActive(QString typeToCheck);
    bool maxNotifications();

signals:
    void notificationNumberChanged();

private:

    void findNextOpenIndex();
    MGConfItem* notificationSettings[MAXNOTIFICATIONS];
    QMap<int,QString> settingsNameMap;
    QMap<QString,int> currentNotificationMap;
    int numberOfNotifications;
    int currentIndex;
};

#endif // NOTIFICATIONTYPES_H
