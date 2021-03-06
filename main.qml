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

    ///This is bad. FIXME!!!!!
    orientationLock: 5
    property bool retranslate: true
    property string currentPageTitle

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
        else if(mainWindow.call.length > 0 ) {
            var cmd = mainWindow.call[0];
            var cdata = mainWindow.call[1];
            settingsModel.reloadModel();
            showPage(cmd, cdata)
        }
        else topView = defaultSettingsPage
    }

    onBookMenuTriggered: {
        console.log("loading translation: " + settingsModel.get(index).translation)
        translator.catalog = settingsModel.get(index).translation
        window.currentPageTitle = settingsModel.get(index).name
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
                window.switchBook(blankPageComponent)
                window.replacePage(blankPageComponent)
                pageLoaderTimer.start();
            }
        }
    }

    Labs.Translator {
        id: translator
    }

    Timer {
        id: pageLoaderTimer
        interval: 1
        repeat: false
        onTriggered: {
            var component = Qt.createComponent(topView)
            if(component.status == Component.Error) {
                console.log("error loading settings page: " + component.errorString())
            }
            window.replacePage(component)
        }
    }

    Component {
        id: blankPageComponent
        Blankness {
            pageTitle: currentPageTitle
        }
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

    function showPage(page, args) {
        var cmd = page;
        var cdata = args;

        console.log("Remote Call: " + cmd + " - " + cdata);
        if (cmd == "showPage")	{
            var page = cdata.split(",")[0];

            if(page == "settings" || page == "") {
                topView = defaultSettingsPage
                return;
            }

            for(var i=0; i< settingsModel.count; i++) {
                var app = settingsModel.get(i).id
                if(page == app ) {
                    translator.catalog = settingsModel.get(i).translation
                    var payloadFile  = settingsModel.get(i).path
                    window.applicationData = cdata
                    topView = payloadFile
                }
            }
        }
    }

    Connections {
        target: mainWindow
        onCall: {
            var cmd = parameters[0];
            var cdata = parameters[1];

            showPage(cmd,cdata)
        }
    }

    Component {
        id: declarativeComponent
        DeclarativeComponent {

        }
    }
}

