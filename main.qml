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

Window {
    id: window

    property bool retranslate: true

    Connections {
        target:  mainWindow
        onRetranslateUi: {
            /// This is a trick to cause retransation to happen
            window.retranslate = !window.retranslate
            window.retranslate = !window.retranslate
            /// We also reset the model
            settingsModel.reloadModel()
            /// reload the current page
            var component = Qt.createComponent(topView)
            if(component.status == Component.Error) {
                console.log("error loading settings page: " + component.errorString())
                return;
            }
            replacePage(component)
        }
    }

    toolBarTitle: window.retranslate ? qsTr("Settings"): ""

    property string defaultSettingsPage: "General/GeneralSettings.qml"
    property variant applicationData
    property string topView

    property string currentBookKey: "currentBook" //The settings book the user user is using T_IGNOREME
    property string bookSaved: "bookSaved" //Flag to check if a book has been saved T_IGNOREME
    property bool restoreFinished: !mainSaveRestoreState.restoreRequired

    //property bool appsChanged: false
    //property bool translated: false

    //bookMenuModel: settingsModel.settingsApps
    bookMenuModel: settingsModel
    automaticBookSwitching: false
    bookMenuActive: false

    Component.onCompleted: {
        if(mainSaveRestoreState.restoreRequired && mainSaveRestoreState.value(bookSaved)) {
            topView = mainSaveRestoreState.value(currentBookKey);
            restoreFinished = true;
            mainSaveRestoreState.setValue(bookSaved,false);
            mainSaveRestoreState.sync();
        }
        else topView = defaultSettingsPage
    }

    onBookMenuTriggered: {
        translator.catalog = settingsModel.get(index).translation
        topView = settingsModel.get(index).path
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
        onSaveRequired: {
            setValue(currentBookKey,topView);
            setValue(bookSaved,true);
            sync();
        }
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
                    topView = defaultSettingsPage
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
}

