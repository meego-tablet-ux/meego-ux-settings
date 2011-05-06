/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

import Qt 4.7
import MeeGo.Labs.Components 0.1 as Labs
import MeeGo.Components 0.1
import MeeGo.Panels 0.1

Labs.ApplicationPage{

    id: container

    title: qsTr("Panels")
    anchors.fill: parent

    PanelProxyModel{
            id: panelModel
            sortType: PanelProxyModel.SortTypeDefaultIndex
    }

    Item {
        id: contentArea
        parent: container.content
        anchors.fill: parent
//        width: scene.content.width
//        height: scene.content.height
//        anchors.centerIn: scene.content

//        Image {
//            id: networkConnectionsLabel
//            width: parent.width
//            source: "image://theme/settings/subheader"

//            Text{
//                anchors.left: parent.left
//                anchors.leftMargin: 10
//                text: qsTr("Network Connections");
//                font.pixelSize: theme_fontPixelSizeLarge
//                height: parent.height
//                width: parent.width
//                elide: Text.ElideRight
//                verticalAlignment: Text.AlignVCenter
//            }
//        }

        ListView {
            // parent: container.content
            //anchors.fill: parent
            width: parent.width
//            anchors.top: parent.top
//            height: parent.height - headerRect.height
//            anchors.topMargin: 2
//            clip: true
            anchors.fill: parent
            model: panelModel
            delegate:panelDelegate
            focus: true
            interactive: (contentHeight > height)
        }

        Component {
            id: panelDelegate
            Item {
                width: parent.width;
                height: imgPanel.height

                Image {
                    id: imgPanel
                    source: "image://theme/settings/btn_settingentry_up"
                    width: parent.width
                }

                Text {
                    id: titleText
                    text: displayName
                    anchors.left: parent.left
                    anchors.leftMargin: 20
                    anchors.right: tbPanel.left
                    anchors.rightMargin: 12
                    color: theme_fontColorNormal
                    font.pixelSize: theme_fontPixelSizeLarge
                    anchors.verticalCenter: parent.verticalCenter
                    wrapMode: Text.NoWrap
                    elide: Text.ElideRight
                }

                ToggleButton {
                    id: tbPanel
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    anchors.rightMargin: 20
                    visible: allowHide
                    on: isVisible
                    onToggled: {
                        panelObj.IsVisible = isOn;
                    }
                }
            }
        }
    }
}
