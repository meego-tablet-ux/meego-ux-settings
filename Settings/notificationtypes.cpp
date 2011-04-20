/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include "notificationtypes.h"

NotificationTypes::NotificationTypes(QObject *parent) :
        QObject(parent), notificationSettings(NULL)
{
    notificationSettings = new MGConfItem(SETTINGSGCONFKEY, this);
    if (notificationSettings->value() != QVariant::Invalid)
        currentNotificationList = notificationSettings->value().toStringList();

    addDefaultNotifications();

    numberOfCustomNotifications = currentNotificationList.size() - NUMBEROFDEFAULTS;
}

void NotificationTypes::addType(QVariant typeToAdd)
{
    if (numberOfCustomNotifications != MAXNOTIFICATIONS && !currentNotificationList.contains(typeToAdd.toString()))
    {
        currentNotificationList.append(typeToAdd.toString());
        numberOfCustomNotifications++;
        notificationSettings->set(currentNotificationList);

        emit notificationNumberChanged();
    }
}

void NotificationTypes::removeType(QVariant typeToRemove)
{
    if (currentNotificationList.contains(typeToRemove.toString()))
    {
        currentNotificationList.removeAll(typeToRemove.toString());
        notificationSettings->set(currentNotificationList);
        numberOfCustomNotifications--;

        emit notificationNumberChanged();
    }
}

bool NotificationTypes::isActive(QString typeToCheck)
{
    if (currentNotificationList.contains(typeToCheck))
        return true;

    return false;
}

bool NotificationTypes::maxNotifications()
{    
    return (numberOfCustomNotifications >= MAXNOTIFICATIONS);
}

void NotificationTypes::addDefaultNotifications()
{
    if (!currentNotificationList.contains("x-nokia.call"))
        currentNotificationList.append("x-nokia.call");

    if (!currentNotificationList.contains("x-nokia.message.arrived"))
        currentNotificationList.append("x-nokia.message.arrived");

    if (!currentNotificationList.contains("x-nokia.message"))
        currentNotificationList.append("x-nokia.message");
}
