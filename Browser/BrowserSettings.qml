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

Labs.ApplicationPage {
    id: bsContainer
    title: qsTr("Browser")
    anchors.fill: parent
		
    BrowserSettingModel {
        id:settings
    }
    property int vkbheight: 0
    property variant current: bsContainer
    Connections {
        target: mainWindow
        onVkbHeight: {
            var map = current.mapToItem(bsContainer, 0, 0);
            vkbheight = height;
            if ((bsContainer.height - map.y - current.height) < vkbheight) 
                flickableContent.contentY += vkbheight - (bsContainer.height - map.y - current.height) + 5
        }
    }

    Flickable
    {
        id: flickableContent
        anchors.top:bsContainer.content.top
        anchors.left: bsContainer.content.left
        contentHeight:settingGroups.height + restoreButton.height + 50
        width:bsContainer.content.width
        height: bsContainer.content.height
        z:-100
        Column
        {
            id: settingGroups
            //anchors.fill:parent
            anchors.top: parent.top
            anchors.left: parent.left
            width: parent.width
            height: childrenRect.height
            spacing: 5
            StartupGroup {
                id: startupGroup
                width: parent.width
                //state: "expanded"
            }

            HomePageGroup {
                id: homepageGroup
                width: parent.width
                //state: "expanded"
            }

            BookmarkGroup {
                id: bookmarkGroup
                width: parent.width
                //state: "expanded"
            }

            SearchGroup {
                id:searchGroup
                width:parent.width
                //state: "expanded"
            }

            NetworkingProxyGroup {
                id:networkingGroup
                width: parent.width
                //state: "expanded"
            }

            /*SyncGroup {
                id:syncGroup
                width:parent.width
            }*/

            PasswordGroup {
                id:passwordGroup
                width: parent.width
            }
            
	    PrivacyGroup {
                id:privacyGroup
                width: parent.width
            }
            ContentSettingGroup {
                id:conentSettingGroup
                width: parent.width
            }
        }// settingGroups column

        //Restore button
        MeeGo.Button {
            id:restoreButton
            width: 240
            height: 60
            text: qsTr("Restore to default")
	    font.pixelSize: theme_fontPixelSizeLarge
	    textColor: theme_buttonFontColor
            anchors.top: settingGroups.bottom
	    anchors.topMargin: 20
            anchors.right: parent.right
            anchors.rightMargin:15

            Component {
                id: dlgComponent
                ConfirmDialog {
                    id: confirmDialog
                    title: qsTr("Restore to default?")
                    message: qsTr("Are you sure you want to restore default settings?")
                    z:100
                    onDialogResponse: {
                        if(accepted) {
                            settings.RestoreDefaultValues();
                        }
                    }
                }
            }

            onClicked: {
                //confirmDialogLoader.sourceComponent = restoreToDefaultComponent;
                var dlg = dlgComponent.createObject(bsContainer)
                dlg.visible = true
            }
        }

    } // Flickr
}
