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

GroupBox {
    id: syncGroup
    title.text: qsTr("Sync")
    width: parent.width

    loader.sourceComponent: syncComponent
    infoText: qsTr("Not set up yet")

//    QtObject {
//        id: syncStatus
//        property int syncStopped: 0
//        property int requestSetupSync: 1
//    }

    // Popup dialog for confirming stop syncing
    Component{
        id: confirmStopDialogComponent
        ConfirmDialog {
            id: confirmDialog
            title: qsTr("Stop Syncing")
            message: {
                qsTr("Are you sure about stopping this account from syncing?")
            }
            onDialogResponse: {
                if(accepted) {
                    if(settings.syncState != BrowserSettingModel.SyncStopped){
                        settings.syncState = BrowserSettingModel.SyncStopped
                    }
                }
            }
        }
    }

    // Popup dialog for login
    Component {
        id:loginDialogComponent
        LoginDialog
        {
            id:loginDialog
            onDialogResponse: {
                if(accepted) {
                    settings.username = loginDialog.username
                    settings.password = loginDialog.password
                    settings.syncState = BrowserSettingModel.RequestSetupSync
                }
            }
        }
    }

    Component
    {
        id: syncComponent
        Column
        {
            id: homepageColumn
            //anchors.fill:parent
            width: syncGroup.width
            height: syncInfoText.height + syncStartStopButton.height + 20
            spacing:10

            Text {
                id:syncInfoText
                width: parent.width -40
                height:50
                verticalAlignment:Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                text: qsTr("You are not set up sync your browser data with another computer.")
                color: theme_fontColorNormal
                font.pixelSize: theme_fontPixelSizeLarge
                wrapMode: Text.Wrap
            }

            Item {
                id:syncStartStopButton
                width: parent.width/3
                height: 45
                anchors.horizontalCenter: parent.horizontalCenter
                property alias text: textButton.title

                Button {
                    id: textButton
                    anchors.fill:parent
                }

                state: {
                    if(settings.syncState == BrowserSettingModel.SyncStopped){
                        return "start";
                    }
                    if(settings.syncState == BrowserSettingModel.ResponseAuthError) {
                        return "start"
                    }
                    return "stop"
                }

                states: [
                    State {
                        name: "start"
                        PropertyChanges {
                            target: textButton
                            title: "Setup Sync..."
                            onClicked: {
                                var loginDlg = loginDialogComponent.createObject(container)
                                loginDlg.visible = true
                            }
                        }
                    },
                    State {
                        name: "stop"
                        PropertyChanges {
                            target: textButton
                            title:"Stop synced to this account"
                            onClicked: {
                                var stopDlg = confirmStopDialogComponent.createObject(container)
                                stopDlg.visible = true;
                            }
                        }
                    }
                ]
            }// sync button item

            Connections {
                target: settings
                onSyncStatusChanged: {
                    if(settings.syncState == BrowserSettingModel.RequestSetupSync) {
                        homepageColumn.state = "request_setup";
                    } else if(settings.syncState == BrowserSettingModel.ResponseAuthError) {
                        homepageColumn.state = "auth_error";
                    } else if(settings.syncState == BrowserSettingModel.ResponseSyncingNow) {
                        homepageColumn.state = "syncing";
                    } else if(settings.syncState == BrowserSettingModel.ResponseSyncDone) {
                        homepageColumn.state = "sync_done";
                    } else homepageColumn.state = "stopped"
                }
            }

            state: {
                if(settings.syncState == BrowserSettingModel.SyncStopped ||
                   settings.syncState == BrowserSettingModel.RequestStopSync) {
                    return "stopped"
                } else if(settings.syncState == BrowserSettingModel.ResponseAuthError){
                    return "auth_error";
                } else if(settings.syncState == BrowserSettingModel.ResponseSyncingNow) {
                    return "syncing";
                } else if(settings.syncState == BrowserSettingModel.ResponseSyncDone) {
                    return "sync_done"
                } else if(settings.syncState == BrowserSettingModel.RequestSetupSync){
                    return "request_setup"
                }
            }

            states : [
                State {
                    name: "stopped"
                    PropertyChanges {
                        target: syncInfoText
                        text: qsTr("You are not set up sync your browser data with another computer.")
                    }
                    PropertyChanges {
                        target: syncStartStopButton
                        state: "start"
                    }
                    PropertyChanges {
                        target: syncGroup
                        infoText: qsTr("Not set up yet")
                    }
                },
                State {
                    name: "request_setup"
                    PropertyChanges {
                        target: syncInfoText
                        text: qsTr("%1 %2 ").arg("Your browser data will be synced to").arg(settings.username);
                    }
                    PropertyChanges {
                        target: syncStartStopButton
                        state: "stop"                        
                    }
                    PropertyChanges {
                        target: syncGroup
                        infoText: "Waiting to sync ..."
                    }
                },
                State {
                    name: "auth_error"
                    PropertyChanges {
                        target: syncInfoText
                        text: qsTr("Invalid username or password.")
                        color: "red"
                    }
                    PropertyChanges {
                        target: syncStartStopButton
                        state: "start"
                    }
                },
                State {
                    name: "syncing"
                    PropertyChanges {
                        target: syncInfoText
                        text: qsTr("Please wait for a moment ...");
                    }
                    PropertyChanges {
                        target: syncStartStopButton
                        state: "stop"
                    }
                    PropertyChanges {
                        target: syncGroup
                        infoText: qsTr("Syncing now...")
                    }
                },
                State {
                    name: "sync_done"
                    PropertyChanges {
                        target: syncInfoText
                        text: qsTr("Synced to %1. Last synced: %2").arg(settings.username).arg(settings.lastSyncedTime);
                    }
                    PropertyChanges {
                        target: syncStartStopButton
                        state: "stop"
                    }
                    PropertyChanges {
                        target: syncGroup
                        infoText: qsTr("Last synced time: %1").arg(settings.lastSyncedTime)
                    }
                }
            ]
        } // sync Column
    } // sync component
}

