/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

import Qt 4.7
import MeeGo.Labs.Components 0.1
import MeeGo.Settings 0.1

ExpandingBox {
    id: container
    detailsComponent: capabilitiesComponent

    property string name: ""
    property bool connected: false
    property string dbuspath: ""
    property string hwaddy: "XX:XX:XX:XX:XX:XX"
    property variant uuids: []
    property BluetoothDevicesModel bluetoothdevicemodel
    property BluetoothDevice device: bluetoothdevicemodel.device(dbuspath)

    Image {
        id: bluetoothIcon
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.margins: 10
        source: "image://meegotheme/icons/settings/"+container.device.icon
        height: container.containerHeight - 20
        fillMode: Image.PreserveAspectFit
        onStatusChanged: {
            if(status == Image.Error) {
		source = "image://meegotheme/icons/settings/device-bluetooth-default"
            }
        }
    }

    Text {
        id: mainText
        anchors.margins: 10
        anchors.top: parent.top
        anchors.left: bluetoothIcon.right
        text: name
    }

    Component {
        id: profileButton
        Button {
            property string uuid

            width: 200
            height: 50

        }
    }

    Component {
        id: audioButtonComponent
        Button {
            width: parent.width
            height: 50
            title: connected ? qsTr("Disconnect Audio"): qsTr("Connect Audio")
            property bool connected: container.device.audioConnected

            onConnectedChanged: {
                if(connected){
                    title = qsTr("Disconnect Audio")
                }

                else{
                    title = qsTr("Connect Audio")
                }
            }

            onClicked: {
                if(connected) {
                    container.device.disconnectAudio();
                }
                else {
                    container.device.connectAudio();
                }
            }
        }
    }

    Component {
        id: inputButtonComponent
        Button {
            width: parent.width
            height: 50
            title: connected ? qsTr("Disconnect"): qsTr("Connect")
            property bool connected: container.device.inputConnected

            onConnectedChanged: {
                if(connected){
                    title = qsTr("Disconnect")
                }

                else{
                    title = qsTr("Connect")
                }
            }

            onClicked: {
                if(connected) {
                    container.device.disconnect();
                }
                else {
                    container.device.connectInput();
                }
            }
        }
    }

    Component {
        id: panButtonComponent
        Item {
            Button {
                width: parent.width
                height: 50
                title: connected ? qsTr("Disconnect Internet"): qsTr("Connect Internet")
                property bool connected: false

                onClicked: {

                }
            }
        }
    }

    Component {
        id: capabilitiesComponent
        Item {
            id: capabilitiesItem

            width: parent.width
            height: profileButtonsColumn.height + removeButton.height
            anchors.horizontalCenter: parent.horizontalCenter

            Component.onCompleted: {
                console.log("getting device for " + dbuspath)
                container.device = bluetoothdevicemodel.device(dbuspath)
            }

            Connections {
		target: container
		onUuidsChanged: {
		    console.log("grabbed property changed for device: "+ container.device.name)
                    profileButtonsColumn.populateList();
                }
            }

            onHeightChanged: {
                console.log("bt expanding area height changed " + height)
            }
            Row {
                height: childrenRect.height
                spacing: 10
                Column {
                    id: profileButtonsColumn
		    width: capabilitiesItem.width / 2 - 10

                    Text {
                        text: qsTr("Connect Actions")
                        height: 50
                        width: 200

                        Component.onCompleted: {
                            profileButtonsColumn.populateList();
                        }
                    }

                    function populateList() {
                        console.log("device: "+ name)
                        var list = container.device.profiles;
                        var hasAudio = false;
                        var count=0
                        for(var i=0;i < list.length;i++) {
                            console.log(container.device.name + ": " + list[i])
                            if(!hasAudio && (list[i] == "00001108-0000-1000-8000-00805f9b34fb" ||
                                             list[i] == "0000110b-0000-1000-8000-00805f9b34fb")) {
                                //audio
                                var button = audioButtonComponent.createObject(profileButtonsColumn);
                                hasAudio = true
                                count ++;
                            }
                            else if(list[i] == "00001116-0000-1000-8000-00805f9b34fb") {
                                //internets nap profile
                                var button = panButtonComponent.createObject(profileButtonsColumn);
                                count ++;
                            }
                            else if(list[i] == "00001124-0000-1000-8000-00805f9b34fb") {
                                inputButtonComponent.createObject(profileButtonsColumn);
                                count ++;
                            }
                        }

                        profileButtonsColumn.visible = (count > 0)
                    }
                }
                Column {
                    id: manageColumn
                    width: capabilitiesItem.width / 2 - 10
                    Text {
                        text: qsTr("Manage")
                        height: 50
                        width: 200
                    }

                    Button {
                        id: removeButton
                        title: qsTr("Remove")
                        height: 50
                        width: parent.width

                        onClicked: {
                            device.unpair();
                        }
                    }
                }
            }
        }
    }
}


