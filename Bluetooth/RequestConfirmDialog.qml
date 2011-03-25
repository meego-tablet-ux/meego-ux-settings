/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

import Qt 4.7
import MeeGo.Labs.Components 0.1

Column {
    id: container
    width: parent.width

    signal replyRequestConfirmation(bool reply)

    property string deviceName: ""
    property string key: ""

    Component.onCompleted: {
        console.log("request  confirm dialog height: " + container.height)
    }

    Text {
        id: textlabel
        width: parent.width
        height: 100
        horizontalAlignment: Text.AlignHCenter
        text: qsTr("Pair with %1 with key %2?").arg(deviceName).arg(key)
    }

    Item {
        id: buttonGroup
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width / 2
        height: 50

        Button {
            id: acceptButton
            anchors.left: parent.left
            width: 100
            height: parent.height
            title: qsTr("Accept")
            color: "green"
            onClicked: {
                replyRequestConfirmation(true);
            }
        }

        Button {
            id: rejectButton
            color: "red"
            anchors.right: parent.right
            width: 100
            height: parent.height
            title: qsTr("Reject")
            onClicked: {
                replyRequestConfirmation(false);
            }
        }
    }
}
