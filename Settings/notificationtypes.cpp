/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include "notificationtypes.h"
#define MAXNOTIFICATIONS 4

NotificationTypes::NotificationTypes(QObject *parent) :
        QObject(parent), notificationSettings("MeeGo", "Settings")
{
    settingsNameMap[0] = "lockscreen/visibleNotification1";
    settingsNameMap[1] = "lockscreen/visibleNotification2";
    settingsNameMap[2] = "lockscreen/visibleNotification3";
    settingsNameMap[3] = "lockscreen/visibleNotification4";

    QString notificationType;

    for (int i = 0; i < MAXNOTIFICATIONS; i++)
    {
        notificationType = notificationSettings.value(settingsNameMap[i],"").toString();

        if (notificationType != "")
        {
            numberOfNotifications++;
            currentNotificationMap.insert(notificationType, i);
        }
    }

    findNextOpenIndex();
}

void NotificationTypes::addType(QString typeToAdd)
{

    if (numberOfNotifications != MAXNOTIFICATIONS)
    {
        notificationSettings.setValue(settingsNameMap[currentIndex], typeToAdd);

        numberOfNotifications++;
        currentNotificationMap.insert(typeToAdd,currentIndex);
        findNextOpenIndex();

        emit notificationNumberChanged();
    }
}

void NotificationTypes::removeType(QString typeToRemove)
{

    if (currentNotificationMap.contains(typeToRemove))
    {
        int indexToRemove = currentNotificationMap[typeToRemove];

        notificationSettings.setValue(settingsNameMap[indexToRemove], "");

        numberOfNotifications--;
        currentIndex = indexToRemove;
        currentNotificationMap.remove(typeToRemove);

        emit notificationNumberChanged();
    }
}


void NotificationTypes::findNextOpenIndex()
{
    currentIndex = -1;

    for (int i = 0; i < MAXNOTIFICATIONS; i++)
    {
        if (notificationSettings.value(settingsNameMap[i], "").toString() == "")
        {
            currentIndex = i;
            i = MAXNOTIFICATIONS;
        }
    }

}

bool NotificationTypes::isActive(QString typeToCheck)
{
    if (currentNotificationMap.contains(typeToCheck))
        return true;

    return false;
}

bool NotificationTypes::maxNotifications()
{
    return (numberOfNotifications == MAXNOTIFICATIONS);
}
