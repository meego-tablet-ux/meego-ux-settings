/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

import Qt 4.7
import MeeGo.Labs.Components 0.1

Item {
	id: container

	property int topicHeight: 35
	property alias model: listView.model
	signal topicTriggered(string path)
	property alias currentTopic: listView.currentIndex
	property alias interactive: listView.interactive

	ApplicationsModel {
		id: listModel;
		directory: "/usr/lib/meego-ux-settings/"
		customField: "DCP/Part"
		type: "ControlPanelApplet"
	}

	ListView {
		id: listView
		anchors.fill: parent

		model:listModel
		highlight: Rectangle {
			width: listView.width;
			height: container.topicHeight;
			color: "#281832"
		}
		highlightMoveDuration: 1
		delegate: Item {
			id: contentItem
			width: container.width
			height: container.topicHeight

			Image {
				anchors.fill: parent
				source: "images/filter-background.png"
			}

			Image {
				anchors.right: parent.right
				anchors.verticalCenter: parent.verticalCenter
				source: "images/right-arrow.png"
			}

			Text {
				id: contentLabel
				height: container.topicHeight
				width: container.width
				text: name
				font.pixelSize: theme_fontPixelSizeNormal
                                color: theme_fontColorHighlight
				anchors.left: parent.left
				anchors.leftMargin: 10
				verticalAlignment: Text.AlignVCenter
			}

			MouseArea {
				anchors.fill: parent
				onClicked: {
					var path = custom;
					if(custom.indexOf("/")) {
						path = listModel.directory + "/" + path;
					}

					container.topicTriggered(path)
				}
			}
		}
	}
}
