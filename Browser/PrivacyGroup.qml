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

GroupBox {
    id: privacyGroup
    title.text: qsTr("Privacy")
    width: parent.width
    loader.sourceComponent: {
        return privacyComponent;
    }

    Component {
        id: dialogComponent
        ConfirmDialog {
            id: confirmDialog
            title: qsTr("Clear Data?")

            property string data: ""

            onDialogResponse: {
                if(accepted) {
                    console.log("clear browsing data")
                    if(settings.needClear) {
                        settings.AddDataItem(data)
                    } else {
                        settings.ResetDataItemsToBeRemoved();
                        settings.AddDataItem(data)
                        settings.needClear = true;
                    }
                }
            }
        }
    }

    Component
    {
        id: privacyComponent

        Column
        {
            id: privacyColumn
            width: privacyGroup.width - 20

            Button {
                id: clearHisotryBtn
                width: parent.width
                height: 50
                font.pixelSize: theme_fontPixelSizeLarge
                text: qsTr("Clear Browsing History")
                onClicked: {
                    var dlg = dialogComponent.createObject(container)
                    dlg.message = qsTr("Are you sure about clearing the browsing history?")
                    dlg.data = "History"
                    dlg.visible = true
                }
            }
            Button {
                id: clearDownloadBtn
                width: parent.width
                height: 50
								font.pixelSize: theme_fontPixelSizeLarge
                text: qsTr("Clear Download History")
                onClicked: {
                    var dlg = dialogComponent.createObject(container)
                    dlg.message = qsTr("Are you sure about clearing the download history?")
                    dlg.data = "Downloads"
                    dlg.visible = true
                }
            }
            Button {
                id: clearCookieBtn
                width: parent.width
                height: 50
                font.pixelSize: theme_fontPixelSizeLarge
                text: qsTr("Clear Cookies and Site Data")
                onClicked: {
                    var dlg = dialogComponent.createObject(container)
                    dlg.message = qsTr("Are you sure about clearing your cookies?")
                    dlg.data = "Cookies"
                    dlg.visible = true
                }
            }
            Button {
                id: clearCacheBtn
                width: parent.width
                height: 50
                font.pixelSize: theme_fontPixelSizeLarge
                text: qsTr("Clear Caches")
                onClicked: {
                    var dlg = dialogComponent.createObject(container)
                    dlg.message = qsTr("Are you sure about clearing your caches?")
                    dlg.data = "Cache"
                    dlg.visible = true
                }
            }
            Button {
                id: clearPasswordBtn
                width: parent.width
                height: 50
                text: qsTr("Clear Saved Passwords")
                font.pixelSize: theme_fontPixelSizeLarge
                onClicked: {
                    var dlg = dialogComponent.createObject(container)
                    dlg.message = qsTr("Are you sure about clearing your saved passwords?")
                    dlg.data = "Passwords"
                    dlg.visible = true
                }
            }
//            Button {
//                id: clearFormDataBtn
//                width: parent.width
//                height: 50
//                text: qsTr("Saved Auto-fill Form Data")
//                onClicked: {
//                    var dlg = dialogComponent.createObject(container)
//                    dlg.message = qsTr("Are you sure clearing form data?")
//                    dlg.data = "FormData"
//                    dlg.visible = true
//                }
//            }

        } // privacyColumn
    } // privacyComponent
}


