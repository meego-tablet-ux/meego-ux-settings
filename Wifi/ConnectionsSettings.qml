/*
* Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

import Qt 4.7
import MeeGo.Labs.Components 0.1
import MeeGo.Components 0.1 as MeeGo
import MeeGo.Settings 0.1
import "helper.js" as WifiHelper

ApplicationPage {
    id: container
    title: qsTr("Connections")
    anchors.fill: parent

    Component.onCompleted: {
        WifiHelper.connmanTechnologies["ethernet"] = qsTr("Ethernet");
        WifiHelper.connmanTechnologies["wifi"] = qsTr("Wi-Fi");
        WifiHelper.connmanTechnologies["bluetooth"] = qsTr("Bluetooth");
        WifiHelper.connmanTechnologies["cellular"] = qsTr("3G");
        WifiHelper.connmanTechnologies["wimax"] = qsTr("WiMAX");
    }

    Flickable {
        id: contentArea
        parent: container.content
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
                source: "image://theme/settings/pulldown_box_2"
                width: parent.width
                Text {
                    id: airplaneLabel
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    text: qsTr("Airplane Mode")
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
                source: "image://theme/settings/subheader"

                Text{
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    text: qsTr("Network Connections");
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
                    source: "image://theme/settings/pulldown_box_2"
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
                                text: WifiHelper.connmanTechnologies[modelData]
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
                                        dtoggle.on = networkListModel.enabledTechnologies.indexOf(modelData) != -1
                                    }
                                }
                            }

                            Image {
                                source: "image://theme/icn_toolbar_button_divider"
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
                source: "image://theme/settings/subheader"
                Text{
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    text: networkListModel.count > 0 ? qsTr("Available Networks"):qsTr("No networks available")
                    font.pixelSize: theme_fontPixelSizeLarge
                    height: parent.height
                    width: parent.width
                    elide: Text.ElideRight
                    verticalAlignment: Text.AlignVCenter
                }
            }

            Column {
                id: availableNetworksList
                width: parent.width
                Repeater {
                    model: networkListModel
                    delegate: availableNetworkItem
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
            defaultRoute:  networkListModel.defaultRoute == model.networkitemmodel
        }
    }
}

