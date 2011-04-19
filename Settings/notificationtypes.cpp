/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include "notificationtypes.h"

NotificationTypes::NotificationTypes(QObject *parent) :
        QObject(parent)
{
    settingsNameMap[0] = "/meego/ux/settings/lockscreen/visibleNotification1";
    settingsNameMap[1] = "/meego/ux/settings/lockscreen/visibleNotification2";
    settingsNameMap[2] = "/meego/ux/settings/lockscreen/visibleNotification3";
    settingsNameMap[3] = "/meego/ux/settings/lockscreen/visibleNotification4";

    QString notificationType;

    for (int i = 0; i < MAXNOTIFICATIONS; i++)
    {
        notificationSettings[i] = new MGConfItem(settingsNameMap[i]);

        if (notificationSettings[i]->value() != QVariant::Invalid)
        {
            notificationType = notificationSettings[i]->value().toString();

                numberOfNotifications++;
                currentNotificationMap.insert(notificationType, i);
        }
    }

    findNextOpenIndex();
}

void NotificationTypes::addType(QVariant typeToAdd)
{
    if (numberOfNotifications != MAXNOTIFICATIONS)
    {
        notificationSettings[currentIndex]->set(typeToAdd);

        numberOfNotifications++;
        currentNotificationMap.insert(typeToAdd.toString(),currentIndex);
        findNextOpenIndex();

        emit notificationNumberChanged();
    }
}

void NotificationTypes::removeType(QVariant typeToRemove)
{

    if (currentNotificationMap.contains(typeToRemove.toString()))
    {
        int indexToRemove = currentNotificationMap[typeToRemove.toString()];

        notificationSettings[indexToRemove]->unset();

        numberOfNotifications--;
        currentIndex = indexToRemove;
        currentNotificationMap.remove(typeToRemove.toString());

        emit notificationNumberChanged();
    }
}


void NotificationTypes::findNextOpenIndex()
{
    currentIndex = -1;

    for (int i = 0; i < MAXNOTIFICATIONS; i++)
    {
        if (notificationSettings[i]->value() == QVariant::Invalid)
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
