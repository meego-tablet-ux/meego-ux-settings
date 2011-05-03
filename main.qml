/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

import Qt 4.7
import MeeGo.Settings 0.1
import MeeGo.Labs.Components 0.1 as Labs
import MeeGo.Components 0.1 as Ux
import Qt.labs.gestures 2.0

Labs.Window2 {
	id: scene
	property variant allSettingsArray: [qsTr("All Settings")];

	filterModel: allSettingsArray.concat(settingsModel.settingsApps)
	filterPayload: settingsModel.settingsAppPaths
	applicationPage: landingPageComponent

	onFilterTriggered: {
		if(index == 0) {
			scene.applicationPage = landingPageComponent
			return;
		}

		translator.catalog = settingsModel.settingsTranslationPaths[index - 1]
		topView = settingsModel.settingsAppPaths[index - 1]

		//scene.applicationPage = Qt.createComponent(payloadFile);
	}

	property string topView

	onTopViewChanged: {
		if(topView != "") {

			console.log(topView.lastIndexOf("xml"))
			if(topView.lastIndexOf("xml") == topView.length - 3) {
				console.log("loading xml setting: " + topView)
				scene.applicationData = topView
				scene.applicationPage = declarativeComponent
			}
			else {
				scene.applicationPage = Qt.createComponent(topView)
			}
		}
	}

	Component.onCompleted: {
		console.log("mainWindow: " + mainWindow)
		console.log("qApp: " + qApp)
	}

	Labs.Translator {
		id: translator
	}

	SettingsModel {
		id: settingsModel
	}

	Labs.ApplicationsModel {
		id: declarativeAppsModel
		directory: "/usr/lib/meego-ux-settings"
	}

	Connections {
		target: mainWindow
		onCall: {
			var cmd = parameters[0];
			var cdata = parameters[1];

			console.log("Remote Call: " + cmd + " - " + cdata);
			if (cmd == "showPage")	{
				var page = cdata.split(",")[0];

				if(page == "settings" || page == "") {
				    scene.applicationPage = landingPageComponent
				    return;
				}

				for(var i=0; i< settingsModel.settingsAppNames.length; i++) {
					if(page == settingsModel.settingsAppNames[i]) {
						var payloadFile  = settingsModel.settingsAppPaths[i]
						scene.applicationData = cdata
						scene.applicationPage = Qt.createComponent(payloadFile);
					}
				}
			}
		}
	}

    Loader {
        id: dialogLoader
        anchors.fill: parent
    }

    Component {
        id: declarativeComponent
        DeclarativeComponent {

        }
    }

	Component {
		id: landingPageComponent
		Labs.ApplicationPage {
			id: landingPage
			title: qsTr("Settings")

			Component.onCompleted: {
				topView=""
			}

			onSearch: {
				settingsModel.filter(needle)
			}

			ListView {
				id: listView
				parent:  landingPage.content
				anchors.fill: parent
				model: settingsModel
				clip: true
				delegate: BorderImage {
					id: container
					source: "image://theme/settings/btn_settingentry_up"
					border.left: 5; border.top: 5
					border.right: 5; border.bottom: 5

					//height: 50
					width: parent.width

					BorderImage {
						id: icon
						anchors.left: parent.left
						anchors.leftMargin: 20
						anchors.verticalCenter: parent.verticalCenter
						source: model.icon != "image://systemicon/" ? model.icon: "image://meegotheme/icons/settings/everyday-settings"
						onStatusChanged: {
						    if(icon.status == Image.Ready) {
								console.log("image width: " + width + " height: " + height)
						    }
						    if(icon.status == Image.Error) {
								///fallback
								icon.source =  "image://meegotheme/icons/settings/everyday-settings"
						    }
						}

						Component.onCompleted: {
							console.log("app: " + model.title + " icon: " + model.icon + " src: " + icon.source)
						}

					}

					Text {
						anchors.left: icon.right
						anchors.leftMargin: 20
						anchors.verticalCenter: parent.verticalCenter
						width: 200
						text: model.title
						height: 30
						font.pixelSize: theme_fontPixelSizeLarge
					}

					GestureArea {
						//id: mouseArea
						anchors.fill: parent

						Tap {
							id: tapArea
							onFinished: {
								translator.catalog = model.translation
								//scene.topView = model.path

								///This is added because of influential people:
								if(topView.lastIndexOf("xml") == topView.length - 3) {
									console.log("loading xml setting: " + topView)
									scene.applicationData = topView
									scene.addApplicationPage(declarativeComponent)

								}
								else {
									scene.addApplicationPage(Qt.createComponent(model.path))
								}
							}
						}
					}

					MouseArea {
						id: mouseArea
						anchors.fill:  parent
					}

					states: [
						State {
							name: "pressed"
							PropertyChanges {
								target: container
								source: "image://theme/settings/btn_settingentry_dn"
							}
							when: mouseArea.pressed
						},
						State {
							name: "normal"
							PropertyChanges {
								target: container
								source: "image://theme/settings/btn_settingentry_up"
							}
							when: !mouseArea.pressed
						}
					]

				}
			}

		}
	}
}

