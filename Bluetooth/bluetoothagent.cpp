/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include <QDeclarativeView>
#include <QDeclarativeContext>

#include "bluetoothagent.h"
#include <asyncagent.h>
#include <bluetoothdevicemodel.h>

BluetoothAgent::BluetoothAgent(QObject *parent) :
	QObject(parent),view( new QDeclarativeView())
{
	agent = new AsyncAgent("/meego/bluetooth/agent", this);

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

	view->setSource(QUrl::fromLocalFile(src));
	QDeclarativeContext *context = view->rootContext();
	context->setContextProperty("agent", (QObject*)this);
	view->show();
}

void BluetoothAgent::requestPasskey(QString device)
{

}

void BluetoothAgent::requestPidCode(QString device)
{

}

void BluetoothAgent::closeView(BluetoothDevice*)
{
	qDebug()<<"release triggered";
	view->close();
}
