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
import MeeGo.Connman 0.1

MeeGo.ExpandingBox {
    id: container
    detailsComponent: capabilitiesComponent

    property int containerHeight: 80

    height: containerHeight

    property string name: ""
    property bool connected: false
    property string dbuspath: ""
    property string hwaddy: "XX:XX:XX:XX:XX:XX"
    property variant uuids: []
    property BluetoothDevicesModel bluetoothdevicemodel
    property BluetoothDevice device: bluetoothdevicemodel.device(dbuspath)

    visible: device.paired

    MeeGo.Theme {
        id: theme
    }

    MeeGo.ThemeImage {
        id: bluetoothIcon
        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.verticalCenter: mainText.verticalCenter

        property string blueToothSource: "image://themedimage/icons/settings/" + container.device.icon

        source: blueToothSource
        defaultSource: "image://themedimage/icons/settings/device-bluetooth-default"
    }

    Text {
        id: mainText
        height: container.containerHeight
        verticalAlignment: Text.AlignVCenter
        anchors.leftMargin: 8
        anchors.left: bluetoothIcon.right
        font.pixelSize: theme.fontPixelSizeNormal
        color: theme.fontColorNormal
        text: name
    }

    Component {
        id: audioButtonComponent
        MeeGo.Button {
            width: parent.width
            height: 50
            text: connected ? qsTr("Disconnect audio"): qsTr("Connect audio")
            property bool connected: container.device.audioConnected

            onConnectedChanged: {
                if(connected){
                    text = qsTr("Disconnect audio")
                }

                else{
                    text = qsTr("Connect audio")
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
        MeeGo.Button {
            property string conntectToString: qsTr("Connect input device")
            property string disconnectToString: qsTr("Disconnect input device")
            width: parent.width
            height: 50
            text: connected ? disconnectToString: conntectToString
            elideText: true
            property bool connected: container.device.inputConnected

            Connections {
                target: container.device
                onInputConnectedChanged: {
                    console.log("Input connected changed (in qml land)");
                    connected = isConnected

                    if(connected){
                        text = disconnectToString
                    }

                    else{
                        text = conntectToString
                    }
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
        MeeGo.Button {
            width: parent.width
            height: 50
            text: connected ? qsTr("Disconnect internet"): qsTr("Connect internet")
            elideText: true
            property bool connected: networkItem.state >= NetworkItemModel.StateReady
            property NetworkItemModel networkItem: networkListModel.service(container.device.name)

            onClicked: {
                connected ? networkItem.disconnectService() : networkItem.connectService()
            }

            NetworkListModel {
                id: networkListModel

                onCountChanged: {
                    networkItem = networkListModel.service(container.device.name)
                }
            }
        }
    }


    Component {
        id: capabilitiesComponent
        Column {
            id: capabilitiesItem

            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 20
            //height: profileButtonsColumn.height + removeButton.height

            Connections {
                target: container
                onUuidsChanged: {
                    profileButtonsColumn.populateList();
                }
            }

            onHeightChanged: {
                console.log("bt expanding area height changed " + height)
            }
            Column {
                height: childrenRect.height
                spacing: 20
                Column {
                    id: profileButtonsColumn
                    width: capabilitiesItem.width - 10
                    property Item audioItem: null
                    property Item napItem: null
                    property Item inputItem: null

                    Connections {
                        target:  container
                        onDeviceChanged: {
                            console.log("text created first!!! " + container.device.name)
                            if(container.device)
                                profileButtonsColumn.populateList();
                        }
                    }

                    Text {
                        text: qsTr("Connect actions")
                        font.pixelSize: theme.fontPixelSizeNormal
                        color: theme.fontColorNormal
                        height: 50
                        width: parent.width
                        elide: Text.ElideRight
                    }

                    function populateList() {
                        console.log("device: "+ name)
                        var list = container.device.profiles;
                        var count=0
                        if(audioItem) audioItem.destroy()
                        audioItem = null;
                        if(napItem) napItem.destroy()
                        napItem = null;
                        if(inputItem) inputItem.destroy()
                        inputItem = null;

                        for(var i=0;i < list.length;i++) {
                            console.log(container.device.name + ": " + list[i])
                            if(audioItem == null && (list[i] == "00001108-0000-1000-8000-00805f9b34fb" ||
                                             list[i] == "0000110b-0000-1000-8000-00805f9b34fb")) {
                                //audio
                                console.log("adding audio button")
                                audioItem = audioButtonComponent.createObject(profileButtonsColumn);
                                audioItem.parent = profileButtonsColumn
                                count ++;
                            }
                            else if(napItem == null && list[i] == "00001116-0000-1000-8000-00805f9b34fb") {
                                //internets nap profile
                                // napItem = panButtonComponent.createObject(profileButtonsColumn);
                                // napItem.parent = profileButtonsColumn
                                // count ++;
                            }
                            else if(inputItem == null && list[i] == "00001124-0000-1000-8000-00805f9b34fb") {
                                //input profile
                                inputItem = inputButtonComponent.createObject(profileButtonsColumn);
                                inputItem.parent = profileButtonsColumn
                                count ++;
                            }
                        }

                        profileButtonsColumn.visible = (count > 0)
                    }
                }
                Column {
                    id: manageColumn
                    width: capabilitiesItem.width - 10
                    Text {
                        text: qsTr("Manage")
                        font.pixelSize: theme.fontPixelSizeNormal
                        color: theme.fontColorNormal
                        height: 50
                        width: parent.width
                        elide: Text.ElideRight
                    }

                    MeeGo.Button {
                        id: removeButton
                        text: qsTr("Remove")
                        height: 50
                        width: parent.width
                        elideText: true

                        onClicked: {
                            device.unpair();
                        }
                    }

                    Text {
                        visible: btHacksGconf.value
                        //height: 50
                        width: parent.width
                        elide: Text.ElideRight
                        text: qsTr("Properties")
                    }

                    Text {
                        visible: btHacksGconf.value
                        //: User will never see this:
                        text: qsTr("Paired: %1").arg(container.device.paired ? "true":"false")
                        //height: 50
                        width: parent.width
                        wrapMode: Text.WordWrap
                        verticalAlignment: Text.AlignVCenter
                    }

                    Text {
                        visible: btHacksGconf.value
                        text: qsTr("Hardware address: %1").arg(container.hwaddy)
                        //height: 50
                        width: parent.width
                        wrapMode: Text.WrapAnywhere
                        verticalAlignment: Text.AlignVCenter
                    }

                    Text {
                        visible: btHacksGconf.value
                        text: qsTr("Icon: %1").arg(container.device.icon)
                        //height: 50
                        width: parent.width
                        wrapMode: Text.WrapAnywhere
                        verticalAlignment: Text.AlignVCenter
                    }

                    Text {
                        visible: btHacksGconf.value
                        text: qsTr("UUIDs: %1").arg(container.uuids.toString())
                        //height: 100
                        wrapMode: Text.WrapAnywhere
                        width: parent.width
                        verticalAlignment: Text.AlignVCenter
                    }

                    Labs.GConfItem {
                        id: btHacksGconf
                        defaultValue: false
                        key: "/meego/ux/settings/bluetoothhacks"
                    }
                }
            }
        }
    }
}


