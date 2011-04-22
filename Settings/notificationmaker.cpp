/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include "notificationmaker.h"


NotificationMaker::NotificationMaker(QObject *parent) :
        QObject(parent)
{

}
void NotificationMaker::makeNotification(QString eventType, QString summary, QString body, QString image)
{
    MNotification newNotification(eventType);
    newNotification.setSummary(summary);
    newNotification.setBody(body);
    newNotification.setImage(image);

    newNotification.publish();

}
