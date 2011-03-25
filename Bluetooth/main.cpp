/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include <QDeclarativeView>
#include <QApplication>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>

#include "bluetoothagent.h"

void daemonize();

int main(int argc, char** argv)
{


	//daemonize();

	QApplication app(argc,argv);

	new BluetoothAgent();

	return app.exec();
}

void daemonize()
{
	int i=0;
	if(getppid() == 1)
	{
		return; // already a daemon
	}
	if((i = fork()) < 0)
	{
		fprintf(stderr, "%s:%s(%d) - fork error: %s", __FILE__, __FUNCTION__, __LINE__, strerror(errno));
		exit(1);
	}
	if(i > 0)
	{
		exit(0);        // parent exits
	}       // child (daemon) continues
	setsid();       // obtain a new process group
	for(i = getdtablesize(); i >= 0; --i)
	{
		close(i);       // close all descriptors
	}
	{       // handle standard I/O
		i = open("/dev/null", O_RDWR);
		dup(i);
		dup(i);
	}
}
