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
import "helper.js" as WifiHelper

ExpandingBox {
    id: container
    //expandedHeight: detailsItem.height
    property NetworkListModel listModel: null
    property QtObject networkItem: null
    property Item page: null
    property int currentIndex
    property string ssid: ""
    property string status: ""
    property int statusint: 0
    property string ipaddy: ""
    property string subnet: ""
    property string gateway: ""
    property string dns: ""
    property string hwaddy: ""
    property string security: ""
    property string method: ""
    property variant nameservers: []

    /// TODO FIXME: this is bad but connman doesn't currently expose a property to indicate whether a service is
    /// the default route or not:
    property bool defaultRoute: false

    Component.onCompleted: {
        WifiHelper.connmanSecurityType["wpa"] = qsTr("WPA");
        WifiHelper.connmanSecurityType["rsn"] = qsTr("WPA2");
        WifiHelper.connmanSecurityType["wep"] = qsTr("WEP");
        WifiHelper.connmanSecurityType["ieee8021x"] = qsTr("RADIUS");
        WifiHelper.connmanSecurityType["psk"] = qsTr("WPA2");
        WifiHelper.connmanSecurityType["none"] = "";

        WifiHelper.IPv4Type["dhcp"] = qsTr("DHCP")
        WifiHelper.IPv4Type["static"] = qsTr("Static")
    }

    onSecurityChanged: {
        securityText.text = WifiHelper.connmanSecurityType[container.security]
    }

    Row {
        spacing: 10
        anchors.left:  parent.left
        anchors.top:  parent.top
        anchors.topMargin: 10
        height: container.containerHeight

        Image {
            id: checkbox
            //anchors.verticalCenter: parent.verticalCenter
            source:  "image://theme/btn_tickbox_dn"
            visible:  container.defaultRoute
        }

        Rectangle {
            id: checkboxFiller
            anchors.fill:  checkbox
            //anchors.verticalCenter: parent.verticalCenter
            color: "transparent"
            visible:  !checkbox.visible
        }


        Image {
            id: signalIndicator
            source: "image://theme/icn_networks"

            states:  [
                State {
                    when: statusint >= NetworkItemModel.StateReady
                    PropertyChanges {
                        target: signalIndicator
                        source: "image://theme/icn_networks_connected"
                    }
                },
                State {
                    when: statusint < NetworkItemModel.StateReady
                    PropertyChanges {
                        target: signalIndicator
                        source: "image://theme/icn_networks"
                    }
                }

            ]
        }

        Text {
            id: mainText
            text: status == "" ? ssid:(ssid + " - " + status)
        }

        Text {
            id: securityText
        }
    }



    onStatusintChanged: {

        if(statusint == NetworkItemModel.StateIdle) {
            status = ""

        }
        else if(statusint == NetworkItemModel.StateFailure) {
            status = qsTr("Failed to Connect")
        }
        else if(statusint == NetworkItemModel.StateAssociation) {
            status = qsTr("Associating")

        }
        else if(statusint == NetworkItemModel.StateConfiguration) {
            status = qsTr("Configuring")

        }
        else if(statusint == NetworkItemModel.StateReady) {
            status = qsTr("Connected")

        }
        else if(statusint == NetworkItemModel.StateOnline) {
            status = qsTr("Connected")
        }
        else {
            console.log("state type: " + statusint + "==" + NetworkItemModel.StateIdle)
        }

        if(statusint == NetworkItemModel.StateIdle || statusint == NetworkItemModel.StateFailure ) {
            detailsComponent = passwordArea
        }
        else if(statusint == NetworkItemModel.StateReady || statusint == NetworkItemModel.StateOnline) {
            detailsComponent = detailsArea
            expanded = false
        }

    }

    /*onExpandedChanged: {
    if(expanded && security == "none" && statusint < NetworkItemModel.StateReady) {
			listModel.connectService(ssid, security, "");
		}
	}*/

    Component {
        id: removeConfirmAreaComponent
        Column {
            id: removeConfirmArea
            width: parent.width
            spacing: 10
            Component.onCompleted: {
                console.log("height: !!!! " + height)
            }

            Text {
                text: qsTr("Do you want to remove %1 ?\nThis action will forget any passwords and you will no longer be automatically connected to %2").arg(networkItem.name).arg(networkItem.name);
                wrapMode: Text.WordWrap
                height: paintedHeight
                width: parent.width
            }

            Row {
                anchors.horizontalCenter: parent.horizontalCenter
                height: childrenRect.height

                Button {
                    id: yesDelete
                    title: qsTr("Yes, Delete")
                    color: "green"
                    width: removeConfirmArea.width / 6
                    height: 50
                    onClicked: {
                        networkItem.removeService();
                        container.expanded = false;
                        container.detailsComponent = detailsArea
                    }
                }
                Button {
                    id: noSave
                    title: qsTr("No, Save")
                    color: "red"
                    width: removeConfirmArea.width / 6
                    height: 50
                    onClicked: {
                        container.expanded = false;
                        container.detailsComponent = detailsArea
                    }
                }
            }
        }
    }

    Component {
        id: detailsArea
        Grid {
            id: settingsGrid
            spacing: 15
            columns: 2
            anchors.top: parent.top
            width: parent.width
            height: childrenRect.height

            Button {
                id: disconnectButton
                title: qsTr("Disconnect")
                height: 50
                width: parent.width / 3
                onClicked: {
                    networkItem.disconnectService();
                    container.expanded = false;
                }
            }

            Button {
                id: removeConnection
                title: qsTr("Remove Connection")
                height: 50
                width: parent.width / 3

                onClicked: {
                    container.detailsComponent = removeConfirmAreaComponent
                }

            }

            Text {
                text: qsTr("Connect by:")
            }

            DropDown {
                id: dropdown
                selectedValue: WifiHelper.IPv4Type[container.method]
                dataModel: [WifiHelper.IPv4Type["dhcp"], WifiHelper.IPv4Type["static"]]

                onSelectionChanged: {
                    console.log("new method value: " + selectedVal)
                    networkItem.method = index == 0 ? "dhcp":"static"
                }
            }

			/*Text {
				id: dropdown
				width: parent.width / 3
				text: WifiHelper.IPv4Type[container.method]
			}*/

			Text {
				text: qsTr("IP Address:")
			}

			Text {
				text: container.ipaddy
				visible:  container.method == "dhcp"
				width: parent.width / 3
			}

			TextEntry {
				id: ipaddyEdit
				width: parent.width / 3
				text: container.ipaddy
				visible: container.method != "dhcp"
				//textInput.inputMask: "000.000.000.000;_"
			}

			Text {
				width: parent.width / 3
				text: qsTr("Subnet Mask:")
			}

			Text {
				text: container.subnet
				visible:  container.method == "dhcp"
				width: parent.width / 3
			}

			TextEntry {
				id: subnetEdit
				width: parent.width / 3
				text: container.subnet
				visible: container.method != "dhcp"
				//textInput.inputMask: "000.000.000.000;_"
			}
			Text {
				width: parent.width / 3
				text: qsTr("Gateway")
			}

			Text {
				text: container.gateway
				visible:  container.method == "dhcp"
				width: parent.width / 3
			}

			TextEntry {
				id: gatewayEdit
				width: parent.width / 3
				text: container.gateway
				visible: container.method != "dhcp"
				//textInput.inputMask: "000.000.000.000;_"
			}
			Text {
				text: qsTr("DNS:")
			}
			Grid {
				id: nameserverstextedit
				width: parent.width
				//height: 20
				columns: 2
				Repeater {
					model: container.nameservers
					delegate: Text {
						width: nameserverstextedit.width / 3
						text: modelData
					}
				}

			}
			Text {
				width: parent.width / 3
				text: qsTr("Hardware Address:")
			}
			Text {
				width: parent.width / 3
				text: container.hwaddy
			}

			Button {
				id: applyButton
				title: qsTr("Apply")
				height: 50
				width: parent.width / 3
				onClicked: {
					//networkItem.method = dropdown.selectedVal
					networkItem.ipaddress = ipaddyEdit.text
					networkItem.netmask = subnetEdit.text
					networkItem.gateway = gatewayEdit.text
				}
			}

			Button {
				id: cancelButton
				title: qsTr("Cancel")
				height: 50
				width: parent.width / 3

				onClicked: {
					container.expanded = false;
				}
			}
		}

	}

    Component {
        id: passwordArea
        Item {
            id: passwordGrid
            width: parent.width
            height: childrenRect.height

            property bool passwordRequired: container.networkItem.type == "wifi" && container.security != "none"

            Column {
                width:  parent.width
                Row {
                    height: childrenRect.height

                    TextEntry {
                        id: passwordTextInput
                        textInput.echoMode: TextInput.Normal
                        visible: passwordGrid.passwordRequired
                        defaultText: qsTr("Type password here")
                        width: parent.width / 2
                        text: container.networkItem.passphrase
                        textInput.inputMethodHints: Qt.ImhNoAutoUppercase

                    }

                    Button {
                        id: setupButton
                        height: 50
                        width: 150
                        title:  qsTr("Setup")
                        visible: container.networkItem.type == "cellular"
                        onClicked: {
                            page.addApplicationPage(cellularSettings)
                        }
                    }

                    Button {
                        id: connectButtonOfAwesome
                        height: 50
                        width: 150
                        title: qsTr("Connect")
                        onClicked: {
                            if(container.networkItem.type == "wifi") {
                                container.networkItem.passphrase = passwordTextInput.text;
                                container.listModel.connectService(container.ssid, container.security, passwordTextInput.text)
                            }
                            else {
                                container.networkItem.connectService();
                            }
                        }
                    }
                }

                Row {
                    height: childrenRect.height

                    CheckBox {
                        id: showPasswordCheckbox
                        visible: passwordGrid.passwordRequired
                        isChecked: true
                        height: 50
                        width: 50

                        onIsCheckedChanged: {
                            if(isChecked) passwordTextInput.textInput.echoMode = TextInput.Normal
                            else passwordTextInput.textInput.echoMode = TextInput.Password
                        }
                    }

                    Text {
                        visible: passwordGrid.passwordRequired
                        text: qsTr("Show Password")
                        width: 100
                        height: 50
                        verticalAlignment: Text.AlignVCenter
                    }
                }
            }





		}
    }

    Component {
        id: cellularSettings
        CellularSettings {
            networkItem: container.networkItem
        }
    }
}



