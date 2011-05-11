/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

import Qt 4.7
import MeeGo.Components 0.1
Item {
    id: container
    visible:true
    property alias username: emailInputBox.text
    property alias password: passwordInputBox.text

    anchors.fill:parent
    signal dialogResponse(bool accepted);

    Rectangle {
	id:fog
	anchors.fill: parent
	color: theme_dialogFogColor
	opacity:theme_dialogFogOpacity
        Behavior on opacity {
            PropertyAnimation { duration: theme_dialogAnimationDuration }
	}
        MouseArea
        {
            anchors.fill: parent
        }
    }

    BorderImage {
        id: dialog
        width: 500
        height: 300
        source: "image://theme/notificationBox_bg"
        anchors.centerIn: parent

        Text {
            id:googleText
            anchors { top: parent.top; topMargin: 20 }
            anchors.horizontalCenter: parent.horizontalCenter
            text: qsTr("Google Account")
            font.weight: Font.Bold
            color: theme_fontColorNormal
            font.pixelSize: theme_fontPixelSizeLarge
        }

        Column {
            id: contentColumn
            width: parent.width
            spacing: 20
            height: parent.height - buttonRow.height
            anchors { top: googleText.bottom; topMargin: 20; left: parent.left; leftMargin: 30 }

            Row {
                id: emailRow
                width: parent.width
                height: childrenRect.height
                spacing: 20

                Text {
                    id: emailText
                    //anchors.top:parent.top
                    //anchors.topMargin: (emailInputBox.height - emailText.height)/2
                    //anchors.left: parent.left;
                    text: qsTr("Email:")
                    width: 100
                    color: theme_fontColorNormal
                    font.pixelSize: theme_fontPixelSizeLarge
                    horizontalAlignment: Text.AlignRight
                    verticalAlignment: Text.AlignVCenter
                }

                InputBox {
                    id:emailInputBox
                    echoMode: TextInput.Normal
                    width: 300
                }
            } // email row

            Row {
                id: passwordRow
                //anchors { top: emailRow.bottom; topMargin: 20; left: parent.left; leftMargin: 30 }
                width: parent.width
                //height: childrenRect.height
                spacing:20
                Text {
                    id: passwordText
                    // anchors.top: emailRow.bottom
                    // anchors.topMargin: (passwordInputBox.height - emailText.height)/2
                    // anchors.left: parent.left
                    text: qsTr("Password:")
                    width: 100
                    font.pixelSize: theme_fontPixelSizeLarge
                    color: theme_fontColorNormal
                    horizontalAlignment: Text.AlignRight
                    verticalAlignment: Text.AlignVCenter
                }

                InputBox {
                    id: passwordInputBox
                    echoMode: TextInput.Password
                    width: 300
                }
            } // password row
        } // column

        Row {
            id: buttonRow
            anchors.bottom: parent.bottom
            //anchors.bottomMargin: 10
            anchors.left: parent.left
            anchors.leftMargin: (parent.width - cancelBtn.width - loginBtn.width - buttonRow.spacing)/2

            width: parent.width
            spacing: 20
            height: 100

            Button {
                id: loginBtn
                width: 190
                height: 50
                text: qsTr("OK")
                font.pixelSize: theme_fontPixelSizeLarge
                onClicked: {
                    if(emailText.text == qsTr("") || passwordInputBox.text == qsTr(""))
                    {
                        googleText.text = qsTr("Please enter your email address and password")
                        return;
                    }
                    container.visible=false;
                    container.dialogResponse(true);
                }
            }
            Button {
                id:cancelBtn
                text: qsTr("Cancel")
                font.pixelSize: theme_fontPixelSizeLarge
                width: 190
                height: 50
                onClicked: {
                    container.visible=false
                    container.dialogResponse(false)
                }
            }
        }
    }
}
