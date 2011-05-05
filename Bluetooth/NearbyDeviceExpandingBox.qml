/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

import Qt 4.7
import MeeGo.Labs.Components 0.1 as Labs
import MeeGo.Settings 0.1

Labs.ExpandingBox {
	id: availableBluetoothItem

	property string deviceName
	property string address
	property string icon

        Image {
            id: bluetoothIcon
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.margins: 10
            source: "image://systemicon/"+icon
            height: availableBluetoothItem.containerHeight - 20
            fillMode: Image.PreserveAspectFit
            onStatusChanged: {
                console.log("icon: " + icon)
                if(status == Image.Error) {
                    source = "image://systemicon/device-bluetooth-default"
                }
            }
        }

	Text {
		id: mainText
		anchors.margins: 10
		anchors.top: parent.top
		anchors.left: bluetoothIcon.right
		text: deviceName
	}
}
