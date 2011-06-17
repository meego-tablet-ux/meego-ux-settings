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
import MeeGo.Connman 0.1

import "helper.js" as WifiHelper

MeeGo.AppPage {
    id: container
    pageTitle: qsTr("Connections")

    property bool finished: false

    Component.onCompleted: {
        WifiHelper.connmanTechnologies["ethernet"] = qsTr("Ethernet");
        WifiHelper.connmanTechnologies["wifi"] = qsTr("Wi-Fi");
        WifiHelper.connmanTechnologies["bluetooth"] = qsTr("Bluetooth");
        WifiHelper.connmanTechnologies["cellular"] = qsTr("3G");
        WifiHelper.connmanTechnologies["wimax"] = qsTr("WiMAX");

        WifiHelper.connmanSecurityType["wpa"] = qsTr("WPA");
        WifiHelper.connmanSecurityType["rsn"] = qsTr("WPA2");
        WifiHelper.connmanSecurityType["wep"] = qsTr("WEP");

        finished = true;
    }

    Flickable {
        id: contentArea
        anchors.fill: parent
        clip: true
        contentWidth: parent.width
        contentHeight: contents.height

        Column {
            id: contents
            width: parent.width
            move: Transition {
                NumberAnimation {
                    properties: "y"
                    easing.type: Easing.OutBounce
                }
            }
            add: Transition {
                NumberAnimation {
                    properties: "opacity"
                    easing.type: Easing.OutBounce
                }
            }

            Image {
                id: offlineArea
                source: "image://themedimage/images/settings/pulldown_box_2"
                width: parent.width
                Text {
                    id: airplaneLabel
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    text: qsTr("Airplane mode")
                    width: 100
                    height: parent.height
                    verticalAlignment: Text.AlignVCenter
                }

                MeeGo.ToggleButton {
                    id: airplaneToggle
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    anchors.rightMargin: 10
                    on: networkListModel.offlineMode
                    onToggled: {
                        networkListModel.setOfflineMode(airplaneToggle.on);
                    }

                    Connections {
                        target: networkListModel
                        onOfflineModeChanged: {
                            airplaneToggle.on = networkListModel.offlineMode
                        }
                    }
                }
            }

            Image {
                id: networkConnectionsLabel
                width: parent.width
                source: "image://themedimage/images/settings/subheader"

                Text{
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    text: qsTr("Network connections");
                    font.pixelSize: theme_fontPixelSizeLarge
                    height: parent.height
                    width: parent.width
                    elide: Text.ElideRight
                    verticalAlignment: Text.AlignVCenter
                }
            }

            Item {
                id: toggleSwitchArea
                width: parent.width
                height: technologiesGrid.height

                Image {
                    id: gridBackground
                    anchors.fill: technologiesGrid
                    source: "image://themedimage/images/settings/pulldown_box_2"
                }

                Grid {
                    id: technologiesGrid
                    width: parent.width
                    height: offlineArea.height * networkListModel.availableTechnologies.count / 2
                    columns: 2
                    Repeater {
                        model: networkListModel.availableTechnologies
                        delegate: Item {
                            width: technologiesGrid.width / 2
                            height: offlineArea.height
                            Text {
                                anchors.top: parent.top
                                anchors.left: parent.left
                                anchors.leftMargin: 10
                                text: finished ? WifiHelper.connmanTechnologies[modelData]: ""
                                width: 100
                                height: parent.height
                                verticalAlignment: Text.AlignVCenter
                            }

                           MeeGo.ToggleButton {
                                id: dtoggle
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.right: parent.right
                                anchors.rightMargin: 10
                                on: networkListModel.enabledTechnologies.indexOf(modelData) != -1
                                onToggled: {
                                    if(dtoggle.on) {
                                        networkListModel.enableTechnology(modelData);
                                    }
                                    else networkListModel.disableTechnology(modelData);
                                }

                                Connections {
                                    target: networkListModel
                                    onEnabledTechnologiesChanged: {
                                        console.log("["+modelData+"]: caught enabled tech signals changed")
                                        dtoggle.on = networkListModel.enabledTechnologies.indexOf(modelData) != -1
                                    }
                                }
                            }

                            Image {
                                source: "image://themedimage/images/icn_toolbar_button_divider"
                                height: parent.height
                                anchors.left: dtoggle.right
                                anchors.leftMargin: 5
                            }

                        }
                    }
                }
            }

            Timer {
                id:timer
                interval: 15000
                repeat: true
                running: true
                onTriggered: {
                    networkListModel.requestScan();
                }
            }

            NetworkListModel {
                id: networkListModel

                Component.onCompleted: {
                    networkListModel.requestScan();
                }
            }

            Image {
                id: availableNetworksLabel
                width: parent.width
                source: "image://themedimage/images/settings/subheader"
                Text{
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    text: qsTr("Available networks")
                    font.pixelSize: theme_fontPixelSizeLarge
                    height: parent.height
                    width: parent.width
                    elide: Text.ElideRight
                    verticalAlignment: Text.AlignVCenter
                }

                MeeGo.Button {
                    text: qsTr("Add network")
                    anchors.right:  parent.right
                    anchors.rightMargin: 10
                    height: parent.height - 10
                    anchors.verticalCenter: parent.verticalCenter
                    //visible: false
                    onClicked: {
                        addNetworkDialog.show()
                    }

                    MeeGo.ModalDialog {
                        id: addNetworkDialog
                        showAcceptButton: true
                        showCancelButton: true
                        title: qsTr("Add network")

                        sizeHintHeight: securityDropdown.selectedIndex > 0 ? 365 : 300  //Expand item height: itemHeight + margin
                        verticalOffset: securityDropdown.selectedIndex > 0 ? 65 : 0     //Keep dialog top position, expand from bottom

                        topMargin: 20
                        leftMargin: 20
                        rightMargin: 20

                        property string ssidHidden
                        property string securityHidden
                        property string securityPassphrase: ""
                        property int itemHeight: 55
                        property bool showSecurityPassphrase: securityDropdown.selectedIndex > 0

                        content: Item {
                            anchors.fill: parent

                            Item {
                                id: leftItems
                                anchors.top: parent.top
                                anchors.bottom: parent.bottom
                                anchors.left: parent.left
                                width: Math.max(networkName.width, securityType.width, securityPassphrase.width)

                                Text {
                                    id: networkName
                                    anchors.top: parent.top
                                    text: qsTr("Network name:")
                                    verticalAlignment: Text.AlignVCenter
                                    height: addNetworkDialog.itemHeight
                                }

                                Text {
                                    id: securityType
                                    anchors.top: networkName.bottom
                                    anchors.topMargin: 10
                                    text: qsTr("Security type:")
                                    verticalAlignment: Text.AlignVCenter
                                    height: addNetworkDialog.itemHeight
                                }

                                Text {
                                    id: securityPassphrase
                                    visible: addNetworkDialog.showSecurityPassphrase
                                    anchors.top: securityType.bottom
                                    anchors.topMargin: 10
                                    text: qsTr("Security passphrase:")
                                    verticalAlignment: Text.AlignVCenter
                                    height: addNetworkDialog.itemHeight
                                }

                            }

                            Item {
                                id: rightItems
                                anchors.top: parent.top
                                anchors.bottom: parent.bottom
                                anchors.left: leftItems.right
                                anchors.leftMargin: 10
                                anchors.right: parent.right

                                MeeGo.TextEntry {
                                    id: ssidEntry
                                    anchors.top: parent.top
                                    anchors.left: parent.left
                                    anchors.right: parent.right
                                    height: addNetworkDialog.itemHeight

                                    onTextChanged: addNetworkDialog.ssidHidden = text
                                }

                                MeeGo.DropDown {
                                    id: securityDropdown
                                    anchors.top: ssidEntry.bottom
                                    anchors.topMargin: 10
                                    anchors.left: parent.left
                                    anchors.right: parent.right
                                    height: addNetworkDialog.itemHeight

                                    model: [ qsTr("none"), qsTr("WPA"), qsTr("WPA2"), qsTr("wep") ]
                                    payload: ["none", "wpa", "rsn", "wep"]
                                    selectedTitle: model[selectedIndex]
                                    selectedIndex: 0
                                    replaceDropDownTitle: true
                                    onTriggered: {
                                        addNetworkDialog.securityHidden = payload[selectedIndex]
                                    }
                                }

                                MeeGo.TextEntry {
                                    id: passPhraseEntry
                                    visible: addNetworkDialog.showSecurityPassphrase
                                    anchors.top: securityDropdown.bottom
                                    anchors.topMargin: 10
                                    anchors.left: parent.left
                                    anchors.right: parent.right
                                    height: addNetworkDialog.itemHeight

                                    textInput.inputMethodHints: Qt.ImhNoAutoUppercase
                                    onTextChanged: addNetworkDialog.securityPassphrase = text
                                }
                            }
                        }
                        onAccepted: {
                            networkListModel.connectService(addNetworkDialog.ssidHidden,
                                                            addNetworkDialog.securityHidden, addNetworkDialog.securityPassphrase)
                        }
                    }
                }
            }

            Column {
                id: availableNetworksList
                width: parent.width

                Text {
                    visible: networkListModel.count == 0
                    text:  qsTr("No networks available")
                    font.pixelSize: theme_fontPixelSizeLarge
                    height: 50
                    width: parent.width
                    elide: Text.ElideRight
                }

                Repeater {
                    model: networkListModel
                    delegate: availableNetworkItem
                    visible: container.x == 0
                }
            }
        }
    }

    Component {
        id: availableNetworkItem
        WifiExpandingBox {
            listModel: networkListModel
            page: container
            width: availableNetworksList.width
            ssid: name
            networkItem: model.networkitemmodel
            currentIndex: model.index
            statusint: model.state
            hwaddy: deviceAddress
            security: model.security
            gateway: model.gateway
            ipaddy: model.ipaddress
            subnet: model.netmask
            method: model.method
            nameservers: model.nameservers
            defaultRoute:  model.defaultRoute
        }
    }
}

