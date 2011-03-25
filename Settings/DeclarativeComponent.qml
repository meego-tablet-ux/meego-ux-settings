/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

import Qt 4.7
import MeeGo.Labs.Components 0.1
import MeeGo.Settings 0.1

Item {
	id: container
	anchors.fill: parent

	property string source//: defaultSource == undefined ? "": defaultSource


	DeclarativeSettingsModel {
		id: declarativeSettings
		document: source

		Component.onCompleted: {

		}

		onStateChanged: {
			if(newState == 2) {
				for(var g=0; g< declarativeSettings.groups.length; g++){
					var group = declarativeSettings.groups[g];
					for(var i = 0;i<group.settings.length; i++){
						if(group.settings[i].type == 1){
							var text = textEdits.createObject(column);
							text.key = group.settings[i].key
							text.title = group.settings[i].title
							if(group.settings[i].value != undefined)
								text.value = group.settings[i].value
							else console.log("error: text value is undefined!")
						}
						else if(group.settings[i].type == 2) {
							///selection type
						}
						else if(group.settings[i].type == 3) {
							var boholean = booleans.createObject(column);
							boholean.key = group.settings[i].key
							boholean.title = group.settings[i].title
							if(group.settings[i].value != undefined)
								boholean.value = group.settings[i].value
							else console.log("error: boolean value is undefined!")
						}
						else if(group.settings[i].type == 4) {

							var integer = integers.createObject(column);
							integer.title = group.settings[i].title
							integer.key = group.settings[i].key
							integer.max = group.settings[i].max
							integer.min = group.settings[i].min

							if(group.settings[i].value != undefined){
								console.log("trying to set integer value to: " + group.settings[i].value)
								integer.value = group.settings[i].value
							}
							else console.log("error: integer value is undefined!")
						}
						else {
							console.log("unknown widget type!!!!")
						}
					}
				}
			}
		}
	}

	Grid {
		anchors.fill: parent
		id: column
		spacing: 20
		columns: 2
	}

	Component {
		id: booleans

		Item {
			property string key: ""
			property string title: ""
			property bool value

			width: parent.width / 2
			height: 50

			Text {
				id: textTitle
				text: title
				anchors.verticalCenter: togglebutton.verticalCenter
			}

			ToggleButton {
				id: togglebutton
				on: value
				anchors.left: textTitle.right
				anchors.leftMargin: 20

				onOnChanged: {
					declarativeSettings.setValue(parent.key, togglebutton.on);
				}
			}
		}
	}

	Component {
		id: integers

		Item {
			property string key: ""
			property string title: ""
			property bool value
			property int max: 100
			property int min: 0

			width:parent.width / 2
			height: 50

			Text {
				id: textTitle
				text: title
				anchors.verticalCenter: slider.verticalCenter
			}

			Slider {
				id: slider
				min: parent.min
				max: parent.max
				anchors.left: textTitle.right
				anchors.leftMargin: 20
				width: parent.width - textTitle.width - 40
				value: parent.value
				onValueChanged: {
					declarativeSettings.setValue(parent.key, slider.value);
				}
			}
		}
	}

	Component {
		id: textEdits
		Item {
			property string key: ""
			property string title: ""
			property string value: ""

			width: parent.width / 2
			height: 50

			Text {
				id: textTitle
				text: title
				anchors.verticalCenter: textEdit.verticalCenter
			}

			TextEdit {
				id: textEdit
				text: value
				anchors.left: textTitle.right
				anchors.leftMargin: 20

				onTextChanged: {
					declarativeSettings.setValue(parent.key,text);
				}
			}
		}
	}
}
