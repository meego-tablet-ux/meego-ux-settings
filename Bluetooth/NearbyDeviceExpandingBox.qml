/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

import Qt 4.7
import MeeGo.Components 0.1
import MeeGo.Settings 0.1
import MeeGo.Bluetooth 0.1

ExpandingBox {
	id: availableBluetoothItem

	property int containerHeight: 80
	//lazyCreation: true
	height: containerHeight

	property string deviceName
	property string address
	property string icon: ""
	property string alias: ""

    ThemeImage {
        id: bluetoothIcon
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.margins: 10
        source: "image://themedimage/icons/settings/"+icon
        defaultSource: "image://themedimage/icons/settings/device-bluetooth-default"

        //height: availableBluetoothItem.containerHeight - 20
    }

	Text {
		id: mainText
		anchors.margins: 10
		anchors.top: parent.top
		anchors.left: bluetoothIcon.right
		anchors.right: parent.right
		anchors.rightMargin: 30
		text: deviceName
		elide: Text.ElideRight
	}
}
