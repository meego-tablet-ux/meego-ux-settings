/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

import Qt 4.7

Item {
	id: container
	anchors.fill: parent
	property string title: "default"
	Rectangle{
		anchors.fill: parent
		color: "slategray"
		Text {
			anchors.centerIn: parent
			text: "This is a placeholder for " + container.title + " settings."
		}
	}


}
