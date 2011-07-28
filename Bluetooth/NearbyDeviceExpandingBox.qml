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
        anchors.left: parent.left
        anchors.verticalCenter: mainText.verticalCenter
        anchors.margins: 20
        property string bluetoothSrc: "image://themedimage/icons/settings/"+icon
        source: bluetoothSrc
        defaultSource: "image://themedimage/icons/settings/device-bluetooth-default"        
    }

	Text {
		id: mainText
		anchors.leftMargin: 8
		verticalAlignment: Text.AlignVCenter
		height:  availableBluetoothItem.containerHeight
		anchors.left: bluetoothIcon.right
		anchors.right: parent.right
		font.pixelSize: theme.fontPixelSizeNormal
		color: theme.fontColorNormal
		anchors.rightMargin: 30
		text: deviceName
		elide: Text.ElideRight
	}
}
