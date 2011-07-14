/*
* Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

import Qt 4.7
import MeeGo.Labs.Components 0.1 as Labs
import MeeGo.Components 0.1 as MeeGo
import MeeGo.Settings 0.1
import MeeGo.Bluetooth 0.1

MeeGo.AppPage{
    id: container
    pageTitle: qsTr("Bluetooth")
    height: contents.height + 10
    BluetoothDevicesModel {
        id: bluetoothModel
        discoverableTimeout: 180 ///three minutes
    }

    Column {
        id: contents
        width: parent.width

        Image {
            width: parent.width
            source: "image://themedimage/images/settings/subheader"
            visible: !bluetoothModel.adapterPresent
            Text{
                anchors.left: parent.left
                anchors.leftMargin: 10
                text: qsTr("No bluetooth adapter present");
                font.pixelSize: theme_fontPixelSizeLarge
                height: parent.height
                width: parent.width
                elide: Text.ElideRight
                verticalAlignment: Text.AlignVCenter
            }
        }

        Column {
            width: parent.width
            visible: bluetoothModel.adapterPresent

            Item {
                id: offlineArea
                height: theme_listBackgroundPixelHeightOne
                width: parent.width
                Text {
                    id: airplaneLabel
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    anchors.right: poweredToggleButton.left
                    anchors.rightMargin: 10
                    text: qsTr("Bluetooth")
                    elide:  Text.ElideRight
                    height: parent.height
                    verticalAlignment: Text.AlignVCenter
                }

                MeeGo.ToggleButton {
                    id: poweredToggleButton
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    anchors.rightMargin: 10
                    Component.onCompleted: {
                        if(bluetoothModel.powered != poweredToggleButton.on)
                            poweredToggleButton.on = bluetoothModel.powered
                    }
                    onToggled: {
                        if(bluetoothModel.powered != poweredToggleButton.on){
                        	bluetoothModel.powered = poweredToggleButton.on
                            poweredToggleButton.enabled = false
                        }
                    }

                    Connections {
                        target: bluetoothModel
                        onPoweredChanged: {
                            if(!bluetoothModel.powered) discoverableTimer.stop();
                            poweredToggleButton.on = bluetoothModel.powered
                            poweredToggleButton.enabled = true


                        }
                    }
                }
            }

            ListSeparator {}
            Item {
                id: bluetoothToggleGrid
                visible: false
                width: parent.width
                height: theme_listBackgroundPixelHeightOne

                Text {
                    id: discoverableLabel
                    text: qsTr("Discoverable (%1)").arg(timeRemaining)
                    anchors.right: visibilityToggleButton.left
                    anchors.rightMargin: 10
                    elide: Text.ElideRight
                    height: parent.height
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    verticalAlignment: Text.AlignVCenter
                    property int timeRemaining : bluetoothModel.discoverable? 180:0
                    states: [
                        State {
                            PropertyChanges {
                                target: discoverableLabel
                                text: qsTr("Discoverable for %n, second(s)", "",timeRemaining)
                            }
                            when: bluetoothModel.discoverable && discoverableLable.timeRemaining > 0
                        }
                    ]

                    Timer {
                        id: discoverableTimer
                        interval: 5000
                        repeat: true
                        onTriggered: {
                            discoverableLabel.timeRemaining -= 5
                        }
                    }

                }

                MeeGo.ToggleButton {
                    id: visibilityToggleButton
                    anchors.right: parent.right
                    anchors.rightMargin: 10
                    anchors.verticalCenter: parent.verticalCenter
                    Component.onCompleted: {
                            visibilityToggleButton.on = bluetoothModel.discoverable
                            if(visibilityToggleButton.on)
                                    discoverableTimer.start();
                    }
                    onToggled: {
			if(bluetoothModel.discoverable != visibilityToggleButton.on)
                        	bluetoothModel.discoverable = visibilityToggleButton.on
                    }

                    Connections {
                        target: bluetoothModel
                        onDiscoverableChanged: {
                            visibilityToggleButton.on = bluetoothModel.discoverable
                            if(bluetoothModel.discoverable){
				bluetoothModel.discoverableTimeout = 180	
                            }
                            else{
				bluetoothModel.discoverableTimeout = 0
                            }
                        }
                    }
                    Connections {
                        target: bluetoothModel
                        onDiscoverableTimeoutChanged: {
                            if(bluetoothModel.discoverableTimeout == 0){
                                visibilityToggleButton.on = false
				discoverableTimer.stop();
                                discoverableLabel.timeRemaining = bluetoothModel.discoverableTimeout
                            }
                            else{
                                visibilityToggleButton.on = true
                                discoverableLabel.timeRemaining = bluetoothModel.discoverableTimeout
				discoverableTimer.start();
                            }
                        }
                    }
                }
	    }
            Image {
                id: devicesLabel
                width: parent.width
                source: "image://themedimage/images/settings/subheader"

                Text{
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    text: qsTr("Paired devices");
                    font.pixelSize: theme_fontPixelSizeLarge
                    height: parent.height
                    anchors.right: addNewDeviceButton.left
                    anchors.rightMargin: 10
                    wrapMode: Text.WordWrap
                    verticalAlignment: Text.AlignVCenter
                }

                MeeGo.Button {
                    id: addNewDeviceButton
                    active: poweredToggleButton.on
                    anchors.right: parent.right
                    anchors.rightMargin: 10
                    anchors.verticalCenter: parent.verticalCenter
                    text: qsTr("Add device")
                    height: parent.height / 1.5
                    //width: 200
                    onClicked: {
                        if (poweredToggleButton.on)
                            addPage(nearbyDevicesComponent);
                    }
                }
            }



            Component {
                id: nearbyDevicesComponent
                NearbyDevices { }
            }

            Column {
                id: deviceList
                width: parent.width
                spacing: 2
                Repeater {
                    model: bluetoothModel
                    delegate: BluetoothDeviceExpandingBox {
                        bluetoothdevicemodel: bluetoothModel
                        name: model.name
                        width: deviceList.width
                        hwaddy: model.address
                        dbuspath: model.path
                        uuids: model.profiles

                        //device: model.bluetoothDevice

                        Connections {
                            target: bluetoothModel
                            onDevicePaired: {
                                console.log("new paired device address:" + device.address + "==" + model.address)
                                if(device.address == model.address){
                                    expanded=true;
                                    device.trusted = true
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
