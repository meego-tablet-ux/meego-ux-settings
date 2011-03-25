/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef BLUETOOTHAGENT_H
#define BLUETOOTHAGENT_H

#include <QObject>

class AsyncAgent;
class QDeclarativeView;
class BluetoothDevice;

class BluetoothAgent : public QObject
{
    Q_OBJECT
public:
    explicit BluetoothAgent(QObject *parent = 0);

signals:
	///required for async agent:
	void release();

public slots:
	///required for async agent:
	void replyRequestConfirmation(bool confirmed);
	void replyPasskey(uint passkey);
	void replyRequestPidCode(QString pidCode);

	void requestConfirmation(QString device, uint code);
	void requestPasskey(QString device);
	void requestPidCode(QString device);

private slots:
	void closeView(BluetoothDevice*);

private:
	AsyncAgent* agent;
	QDeclarativeView *view;
};

#endif // BLUETOOTHAGENT_H
