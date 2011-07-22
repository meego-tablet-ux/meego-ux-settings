/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

import Qt 4.7
import MeeGo.Components 0.1 as MeeGo

Column {
    id: container
    width: parent.width

    signal replyRequestConfirmation(bool reply)

    property string deviceName: ""
    property string key: ""

    Component.onCompleted: {
        console.log("request confirm dialog height: " + container.height)
    }

    Text {
        id: textlabel
        width: parent.width
        height: paintedHeight
        horizontalAlignment: Text.AlignHCenter
        text: qsTr("Pair with %1 with key %2?").arg(deviceName).arg(key)
        wrapMode: Text.WordWrap
        font.pixelSize: theme.fontPixelSizeNormal
        color: theme.fontColorNormal
    }

    Row {
        id: buttonGroup
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 10
        height: 50

        MeeGo.Button {
            id: acceptButton
            height: parent.height
            width:  container.width / 2 - 20
            text: qsTr("Accept")
            onClicked: {
                replyRequestConfirmation(true);
            }
        }

        MeeGo.Button {
            id: rejectButton
            height: parent.height
            width:  container.width / 2 - 20
            text: qsTr("Reject")
            onClicked: {
                replyRequestConfirmation(false);
            }
        }
    }
}
