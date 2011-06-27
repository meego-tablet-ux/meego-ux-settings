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
import MeeGo.Components 0.1
import MeeGo.Ux.Gestures 0.1

UberWindow {
    id: window

    property bool retranslate: true

    Connections {
        target:  mainWindow
        onRetranslateUi: {
            window.retranslate = !window.retranslate
            window.retranslate = !window.retranslate
        }
    }

    toolBarTitle: window.retranslate ? qsTr("Settings"): ""
    property variant allSettingsArray: [qsTr("All Settings")];
    property variant applicationData
    property string topView

    property string currentBookKey: "currentBook" //The settings book the user user is using T_IGNOREME

    property bool restoreFinished: !mainSaveRestoreState.restoreRequired

    //bookMenuModel: settingsModel.settingsApps
    bookMenuModel: settingsModel
    automaticBookSwitching: false
    bookMenuActive: false

    Component.onCompleted: {
        if(mainSaveRestoreState.restoreRequired) {
            topView = mainSaveRestoreState.value(currentBookKey);
            restoreFinished = true;
        } else {
            mainSaveRestoreState.sync();
        }
    }

    onBookMenuTriggered: {
        translator.catalog = settingsModel.get(index).translation
        topView = settingsModel.get(index).path
        mainSaveRestoreState.setValue(currentBookKey,topView);
        mainSaveRestoreState.sync();
    }

    onTopViewChanged: {
        if(topView != "") {
            console.log(topView.lastIndexOf("xml"))
            if(topView.lastIndexOf("xml") == topView.length - 3) {
                console.log("loading xml setting: " + topView)
                window.applicationData = topView
                window.switchBook(declarativeComponent)
            }
            else {

                var component = Qt.createComponent(topView)
                if(component.status == Component.Error) {
                    console.log("error loading settings page: " + component.errorString())
                }
                window.switchBook(component)
            }
        }
    }

    Labs.Translator {
        id: translator
    }

    SettingsModel {
        id: settingsModel
    }

    SaveRestoreState {
        id: mainSaveRestoreState
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
                    window.switchBook(landingPageComponent)
                    return;
                }

                for(var i=0; i< settingsModel.settingsAppNames.length; i++) {
                    if(page == settingsModel.settingsAppNames[i]) {
                        translator.catalog = settingsModel.get(i).translation
                        var payloadFile  = settingsModel.get(i).path
                        window.applicationData = cdata
                        topView = payloadFile
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
        AppPage {
            id: landingPage
            property string scrollDownAmount: "landingScrollAmount" //T_IGNOREME
            pageTitle: qsTr("Settings")

            Component.onCompleted: {
                topView=""
            }

            onActivated: {
                if(window.restoreFinished) {
                    landingPageState.setValue(window.currentBookKey,"");
                    landingPageState.sync();
                }
            }

            onSearch: {
                if(settingsHacksGconf.value)
                    settingsModel.filter(needle)
            }

            Labs.GConfItem {
                id: settingsHacksGconf
                defaultValue: false
                key: "/meego/ux/settings/settingshacks"
            }

            SaveRestoreState {
                id: landingPageState
                onSaveRequired: {
                    setValue(landingPage.scrollDownAmount,listView.contentY);
                    sync();
                }
            }

            ListView {
                id: listView
                //parent:  landingPage.content
                anchors.fill: parent
                model: settingsModel
                clip: true
                Component.onCompleted: {
                    listView.contentY = landingPageState.restoreRequired ? landingPageState.value(landingPage.scrollDownAmount) : 0;
                }

                delegate: ThemeImage {
                    id: container
                    source: "image://themedimage/images/settings/btn_settingentry_up"
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
                                //window.topView = model.path

                                landingPageState.setValue(window.currentBookKey,model.path);
                                landingPageState.sync();

                                ///This is added because of influential people:
                                if(topView.lastIndexOf("xml") == topView.length - 3) {
                                    console.log("loading xml setting: " + topView)
                                    window.applicationData = topView
                                    window.addPage(declarativeComponent)
                                }
                                else {
                                    var component = Qt.createComponent(model.path)
                                    if(component.status == Component.Error) {
                                        console.log("error loading settings page: " + component.errorString())
                                    }

                                    window.addPage(component)
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
                                source: "image://themedimage/images/settings/btn_settingentry_dn"
                            }
                            when: mouseArea.pressed
                        },
                        State {
                            name: "normal"
                            PropertyChanges {
                                target: container
                                source: "image://themedimage/images/settings/btn_settingentry_up"
                            }
                            when: !mouseArea.pressed
                        }
                    ]

                }
            }

        }
    }
}

