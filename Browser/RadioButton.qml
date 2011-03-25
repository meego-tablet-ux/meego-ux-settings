/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

import Qt 4.7

TextBox {
    id: radioBox
    boxText.text: qsTr("Radio Button")
    boxText.anchors.left: radioBox.left
    boxText.anchors.leftMargin: 15
    boxText.horizontalAlignment: Text.AlignLeft
    focus:true

    state: if(checked)"checked"; else "unchecked"

    property bool checked: true
    property alias title: radioBox.boxText
    property alias loader: contentLoader
    property alias iconVisible: radioIcon.visible

    height: 60
    width: 600

    signal clicked();

    Image {
        id: radioIcon
        smooth: true
        source: radioBox.checked?"image://theme/btn_radio_dn" : "image://theme/btn_radio_up"
				anchors.verticalCenter: parent.verticalCenter
        anchors.right: boxText.right
        anchors.rightMargin:7 
        visible: true
    }

    Loader {
        id:contentLoader
        anchors.right: radioIcon.left
        anchors.rightMargin: 15
        anchors.top: parent.top
        //anchors.verticalCenter: parent.verticalCenter
        onLoaded: {
            contentLoader.anchors.topMargin = (radioBox.height - contentLoader.height)/2
        }
    }
    states: [
        State {
            name: "checked"
            PropertyChanges {
                target: radioIcon
                source: "image://theme/btn_radio_dn"
            }
            PropertyChanges {
                target: radioBox                
                checked: true
            }
        },
        State {
            name: "unchecked"
            PropertyChanges {
                target: radioIcon
                source: "image://theme/btn_radio_up"
            }
            PropertyChanges {
                target: radioBox
                checked: false
            }
        }
    ]

    mouseArea.onClicked: {
        radioBox.focus = true
        radioBox.state = radioBox.state == "checked" ? "unchecked" : "checked"
        radioBox.clicked()
    }
}
