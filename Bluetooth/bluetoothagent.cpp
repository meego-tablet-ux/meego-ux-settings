/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include <QDeclarativeView>
#include <QDeclarativeContext>
#include <QGLFormat>

#include "bluetoothagent.h"
#include <asyncagent.h>
#include <bluetoothdevicemodel.h>
#include <launcherwindow.h>

BluetoothAgent::BluetoothAgent(QObject *parent) :
	QObject(parent),view( new LauncherWindow(true,800,480,false,false,false) )
{
	agent = new AsyncAgent("/meego/bluetooth/agent", this);

	QGLFormat format = QGLFormat::defaultFormat();
	format.setSampleBuffers(false);
	format.setSamples(0);
	format.setAlpha(true);
	view->getDeclarativeView()->setViewport(new QGLWidget(format));
	view->getDeclarativeView()->viewport()->setAttribute(Qt::WA_TranslucentBackground);
	view->getDeclarativeView()->setWindowFlags(Qt::FramelessWindowHint);
	view->getDeclarativeView()->setAttribute(Qt::WA_TranslucentBackground);

	BluetoothDevicesModel* bluetoothDevices = new BluetoothDevicesModel(this);

	connect(bluetoothDevices,SIGNAL(devicePaired(BluetoothDevice*)),
			this,SLOT(closeView(BluetoothDevice*)));
}


void BluetoothAgent::replyRequestConfirmation(bool confirmed)
{
	qDebug()<<"reply to RequestConfirmation:"<<confirmed;
	agent->replyRequestConfirmation(confirmed);
}

void BluetoothAgent::replyPasskey(uint passkey)
{
	agent->replyPasskey(passkey);
}

void BluetoothAgent::replyRequestPidCode(QString pidCode)
{
	agent->replyRequestPidCode(pidCode);
}

void BluetoothAgent::requestConfirmation(QString device, uint code)
{
	qDebug()<<"requestConfirmation ftw";
	QString src = "/usr/share/meego-ux-settings/Bluetooth/AgentConfirmView.qml";

	view->getDeclarativeView()->setSource(QUrl::fromLocalFile(src));
	QDeclarativeContext *context = view->getDeclarativeView()->rootContext();
	context->setContextProperty("agent", (QObject*)this);
	view->show();
}

void BluetoothAgent::requestPasskey(QString device)
{
	qDebug()<<"requestConfirmation ftw";
	QString src = "/usr/share/meego-ux-settings/Bluetooth/AgentPasskeyEntryView.qml";

	view->getDeclarativeView()->setSource(QUrl::fromLocalFile(src));
	QDeclarativeContext *context = view->getDeclarativeView()->rootContext();
	context->setContextProperty("agent", (QObject*)this);
	view->show();
}

void BluetoothAgent::requestPidCode(QString device)
{
	qDebug()<<"requestConfirmation ftw";
	QString src = "/usr/share/meego-ux-settings/Bluetooth/AgentPinEntryView.qml";

	view->getDeclarativeView()->setSource(QUrl::fromLocalFile(src));
	QDeclarativeContext *context = view->getDeclarativeView()->rootContext();
	context->setContextProperty("agent", (QObject*)this);
	view->show();
}

void BluetoothAgent::closeView(BluetoothDevice*)
{
	qDebug()<<"release triggered";
	view->close();
}
