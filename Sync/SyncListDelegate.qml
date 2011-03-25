/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

import Qt 4.7
import MeeGo.Labs.Components 0.1
import MeeGo.Sync 0.1

BorderImage {
    id: syncListData
    source: "image://theme/settings/btn_settingentry_up"
    border.left: 5
    border.right: 5
    border.top: 5
    border.bottom: 5

    // Once the list item is ready for use the opacity will change to 1 (opaque),
    // and the MouseArea will become active.
    opacity: ready ? 1 : 0.5

    property bool pressed: false
    property string theUsername: username
    property string thePassword: password

    height: {
        var h = serviceIcon.height;
        if (serviceLabel.height > h)
            h = serviceLabel.height;
        if (scheduledStatus.height > h)
            h = scheduledStatus.height;
        if (arrowRight.height > h)
            h = arrowRight.height

        h;
    }

    width: parent.width

    Component {
        id: syncDetails

        ApplicationPage {
            id: syncDetailsPage
            anchors.fill: parent
            title: model.storage

            onSearch: {
                    console.log("application search query: " + needle)
            }

            SyncDetails {
                id: fnord
                parent:   syncDetailsPage.content
                icon:     model.image
                storage:  model.storage
                service:  model.displayName
                name:     model.name
                username: theUsername
                password: thePassword
            }
        }
    }

    MouseArea {
        anchors.fill: parent

        onClicked: {
            if (!ready) {
                return;
            }

            // Set properties related to the selected sync service in our
            // parent list for use in child pages.
            theUsername = username;
            thePassword = password;

            // Check if credentials have already been set.  If not, pop
            // up the login dialog, store the credentials, and schedule
            // and start the sync.
            if (username == "") {
                theLoginDialog = cardMe.createObject(syncListData);
            } else {
                appPage.addApplicationPage(syncDetails);
            }
        }

        onPressed:  parent.pressed = ready
        onReleased: parent.pressed = false
    }

    function executeOnSignin(u, p) {
        theUsername = u;
        thePassword = p;
        appPage.addApplicationPage(syncDetails);
    }

    Component {
        id: cardMe

        SyncLoginDialog {
            serviceName: displayName
            loginOwner: syncListData
        }
    }

    Item {
        id: serviceIcon
        height: theIcon.height + 10
        width: theIcon.width + 10
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left

        Image {
            id: theIcon
            source: image
            anchors.centerIn: serviceIcon
        }
    }

    Text {
        id: serviceLabel
        anchors.verticalCenter: parent.verticalCenter
        x: 100  // Force alignment of all service labels

        color: theme_fontColorNormal
        font.pixelSize: theme_fontPixelSizeMediumLarge

        //: Arg 1 is the sync service name (e.g. "Yahoo!") and arg 2 is the storage name (e.g. "Contacts" or "Calendar".
        text: qsTr("%1 %2").arg(displayName).arg(storage)
    }

    Item {
        id: scheduledStatus
        height: scheduledIcon.height + 10
        width: scheduledIcon.width + 10
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: arrowRight.left

        Image {
            id: scheduledIcon
            anchors.centerIn: parent
            source: active ? "image://theme/btn_tickbox_dn" : ""
        }
    }

    Item {
        id: arrowRight
        height: rightIcon.height + 10
        width:  rightIcon.width + 10
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right

        Image {
            id: rightIcon
            anchors.centerIn: parent
            source: "image://theme/arrow-right"
        }
    }


    states: [
        State {
            name: "pressed"
            when: syncListData.pressed
            PropertyChanges {
                target: syncListData
                source: "image://theme/settings/btn_settingentry_dn"
            }
        }
    ]
}
