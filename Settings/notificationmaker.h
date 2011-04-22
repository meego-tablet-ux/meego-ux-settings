/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef NOTIFICATIONMAKER_H
#define NOTIFICATIONMAKER_H

#include <QObject>
#include <QDebug>
#include <MNotification>

class NotificationMaker : public QObject
{
    Q_OBJECT

public:
    explicit NotificationMaker(QObject *parent = 0);

public slots:
   void makeNotification(QString eventType, QString summary, QString body, QString image);
   void makeNotificationVoid() {qDebug()<<"BJONES called it";}

private:
   bool junk;
};

#endif // NOTIFICATIONMAKER_H
