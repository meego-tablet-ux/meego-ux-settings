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

Labs.ApplicationPage {
    id: page
    title: qsTr("Add New Device")

    property bool suppress: false

    Component.onCompleted: {
        nearbyDevicesModel.discover(true);
    }

    Component.onDestruction: {
        nearbyDevicesModel.discover(false);
    }

    NearbyDevicesModel {
        id: nearbyDevicesModel

        property bool discovering: false

        Component.onCompleted: {

        }

        onRequestConfirmation: {
            console.log("onRequestConfirm called")
        }

        onRequestPasskey: {
            console.log("onRequestPasskey called")

        }

        onRequestPidCode: {
            console.log("onRequestPidCode called")
        }

        onAdapterPropertiesChanged: {
            //console.log("name: "+name+" value: "+value)
            if(name == "Discovering") {
                discovering = value;
            }
            else if(name == "Discoverable") {
                visibilityToggleButton.toggle(value)
            }
        }

        onNearbyDeviceFound: {
            //console.log("new device: " + nearbyDevicesModel.alias(index))
        }

    }

    Item {
        parent: page.content
        anchors.fill: parent

        /*Text {
        id: nearbyDevicesLabel
			text: qsTr("Nearby Devices:")
			font.pixelSize: theme_fontPixelSizeLarge
			anchors.left: nearbyDevicesList.left
			anchors.leftMargin: 10
			anchors.top: parent.top
			height: 20
			width: 100
		}*/

		ListView {
			id: nearbyDevicesList
			width: parent.width
			height: parent.height
			anchors.horizontalCenter: parent.horizontalCenter
			anchors.top: parent.top
			model: nearbyDevicesModel
			delegate: NearbyDeviceExpandingBox {
				id: availableBluetoothItem
				width: nearbyDevicesList.width
				deviceName: name
				icon: model.icon

				onExpandedChanged: {
					console.log("pairing with device: " + model.address)
					if(expanded) {
						nearbyDevicesModel.discover(false)
						nearbyDevicesModel.pair(model.address)
					}

				}

				Connections {
					target: nearbyDevicesModel
					onRequestConfirmation: {
						console.log("spawning request confirm dialog")
						if(device != deviceName) return;
						availableBluetoothItem.detailsComponent = requestConfirmDialog
						availableBluetoothItem.detailsItem.deviceName = device
						availableBluetoothItem.detailsItem.key = code
					}

					onRequestPasskey: {
						if(device != deviceName) return;
						availableBluetoothItem.detailsComponent = requestPasskeyDialog
						availableBluetoothItem.detailsItem.deviceName = device
					}

					onRequestPidCode: {
						if(device != deviceName) return;
						availableBluetoothItem.detailsComponent = requestPidCodeDialog
						availableBluetoothItem.detailsItem.deviceName = device
						availableBluetoothItem.detailsItem.legacyPair = model.legacyPairing
						console.log(device + " model legacyPairing: " + model.legacyPairing)
					}
				}
			}
		}
    }

    Connections {
        target: nearbyDevicesModel
        onRelease: {
            if(!suppress) {
                page.suppress = true;
                page.close()
            }
        }
    }

    Component {
        id: requestPasskeyDialog
        RequestpasskeyDialog {
            onReplyRequestPasskey: {
                nearbyDevicesModel.replyPasskey(reply)
            }
        }
    }

    Component {
        id: requestPidCodeDialog
        RequestPidCodeDialog {
            onReplyRequestPidCode: {
                nearbyDevicesModel.replyRequestPidCode(reply)
            }
        }
    }

    Component {
        id: requestConfirmDialog
        RequestConfirmDialog {
            onReplyRequestConfirmation: {
                nearbyDevicesModel.replyRequestConfirmation(reply)
            }
        }
    }
}
