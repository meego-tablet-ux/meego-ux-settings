/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

import Qt 4.7

Item
{
    id: box
    property alias mouseArea: mouseArea
    property alias title: txt.text
    property alias boxText: txt

    Image
    {
        id:img
        height:parent.height
        width:parent.width
        fillMode: Image.Stretch
        opacity:1
        //source: "image://theme/settings/subheader"
 
      states: [
			State {
				name: "pressed"
				PropertyChanges {
					target: img 
					source: "image://theme/settings/btn_settingentry_dn"
				}
				when: mouseArea.pressed
			},
			State {
				name: "normal"
				PropertyChanges {
					target: img
					source: ""
				}
				when: !mouseArea.pressed
			}
		]
   }

    Text
    {
        id: txt
        anchors.fill: parent
        text:qsTr("Text")
        color:theme_fontColorNormal
        font.pixelSize: theme_fontPixelSizeLarge
        horizontalAlignment:Text.AlignHCenter
        verticalAlignment:Text.AlignVCenter

    }
		MouseArea {
			id: mouseArea
			anchors.fill: parent
		}

}
