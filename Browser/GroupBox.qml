/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

import Qt 4.7
import MeeGo.Components 0.1

Item
{
    id:group
    property alias mouseArea: mouseArea
    property alias loader: contentLoader
    property alias title: groupBorderImageTitle
    property alias infoText: groupInfoText.text
    property alias info: groupInfoText
    width:parent.width
    height: groupBorderImage.height

    state: "elapsed"

    states: [
        State {
            name: "elapsed"
            PropertyChanges  { target:contentLoader; height: 0; visible:false }
            PropertyChanges  { target:groupBorderImage; height: 60; opacity: 0.7}
            PropertyChanges  { target:expandButton; rotation:0; }
        },
        State {
            name: "expanded"
						PropertyChanges { target:contentLoader; visible:true }
            PropertyChanges { target:expandButton; rotation:180 }
        }
    ]

    transitions: Transition {
        NumberAnimation { properties: "height,visible"; easing.type: Easing.InOutQuart; duration: 300 }
    }

    BorderImage
    {
        id:groupBorderImage
        source: "image://theme/notificationBox_bg"
        opacity:1
        width: parent.width
        border { left: 20; right: 20; top:50; bottom: 10}
        height: 60
        horizontalTileMode: BorderImage.Stretch
        verticalTileMode: BorderImage.Stretch
//        BorderImage {
//            id: bg
//            source: "image://theme/notificationBox_bg_shadow"
//            border { left: 20; right: 20; top: 50; bottom:10 }
//            width: parent.width+3
//            height: parent.height+3
//            //anchors.fill: parent
//        }

        Item {
            id: titleBar
            anchors.top: groupBorderImage.top
            anchors.left: groupBorderImage.left
            width: groupBorderImage.width
            height: 60
            // Info text
            Text {
                id: groupInfoText
                text: qsTr("")
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 70
                font.pixelSize: theme_fontPixelSizeLarge
                color:theme_fontColorNormal
                //style: Text.Outline
                //styleColor: theme_fontColorHighlight
            }
            // Title text
            Text {
                id: groupBorderImageTitle
                //anchors.top: parent.top
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 20
                text: qsTr("group")
                color: theme_fontColorNormal
                font.pixelSize: theme_fontPixelSizeLarge
                font.bold: true

            }

            Image {
                id:expandButton
                source: "image://theme/notes/icn_dropdown_off"
                anchors.top: parent.top
                anchors.topMargin: 23
                anchors.right: parent.right
                anchors.rightMargin: 23
                state: "inactive"

                states: [
                    State {
                        name: "inactive"
                        PropertyChanges {
                            target: expandButton
                            source: "image://theme/notes/icn_dropdown_off"
                            anchors.topMargin: 23
                            anchors.rightMargin: 23
                        }
                    } ,
                    State {
                        name: "active"
                        PropertyChanges {
                            target: expandButton
                            source: "image://theme/notes/icn_dropdown_off"
                            anchors.topMargin: 23 //(50 - expandButton.height)/2
                            anchors.rightMargin: 23
                        }
                    }
                ]
            }
        }
    }

    Loader
    {
        id:contentLoader
        width:groupBorderImage.width - 20
        anchors { top: parent.top; topMargin: 50; left: parent.left; leftMargin:15; right:parent.right; rightMargin:15}
        onLoaded: {
            groupBorderImage.height = item.height + 60
        }
    }

    MouseArea {
        id: mouseArea
        anchors.top:parent.top
        anchors.left:parent.left
        width:parent.width
        height:50
        
        onClicked: {
            if(group.state == "expanded")
                group.state = "elapsed";
            else group.state = "expanded";
            group.focus=true
        }
    }

}
