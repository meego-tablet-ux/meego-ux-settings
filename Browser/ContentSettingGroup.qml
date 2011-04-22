/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

import Qt 4.7
import MeeGo.Settings 0.1

GroupBox {
    id: contentSettingGroup
    title.text: qsTr("Content Settings")
    width: parent.width
    loader.sourceComponent: contentSettingComponent

    function appendChoice(choice) {
        var i;
        for(i = 0; i < choiceModel.count; i++){
            if(choiceModel.get(i).choice == choice){
                return;
            }
        }
        choiceModel.append({"choice": choice})
    }

    function removeChoice(choice) {
        var i = 0;
        for(i = 0; i < choiceModel.count; i++){
            if(choiceModel.get(i).choice == choice){
                choiceModel.remove(i)
                break;
            }
        }
    }

    ListModel {
        id: choiceModel
    }

    ListView {
        id: choiceListView
        model: choiceModel
        anchors.top: contentSettingGroup.top
        anchors.topMargin:15
        anchors.right: contentSettingGroup.right
        anchors.rightMargin: 170
        width: count*80
        spacing: 5
        orientation:ListView.Horizontal
        delegate: Text{
            text: choice + ","
            font.pixelSize: theme_fontPixelSizeLarge
            color:theme_fontColorHighlight
            style: Text.Outline
            styleColor: "gray"
        }
    }

    Component
    {
        id: contentSettingComponent
        Column
        {
            id: contentSettingColumn
//            width: contentSettingGroup.width
            spacing:-5

            Connections {
                target: settings
                onAllowJavascriptChanged: {
                   allowJavascriptCheckbox.state = settings.allowJavascript ? "checked" : "unchecked"
                }
                onAllowImagesChanged: {
                   allowImageCheckbox.state = settings.allowImages ? "checked" : "unchecked"
                }
                onAllowCookiesChanged: {
                   allowCookieCheckbox.state = settings.allowCookies ? "checked" : "unchecked"
                }
                onAllowPopupChanged: {
                   allowPopupCheckbox.state = settings.allowPopup ? "checked" : "unchecked"
                }
            }

            CheckboxButton {
                id: allowJavascriptCheckbox
                width: parent.width
                height: 50
                title.text: qsTr("Allow JavaScript to run")
                state: {
                    if(settings.allowJavascript)
                        return "checked"
                    else return "unchecked"
                }

                onClicked :{
                    settings.allowJavascript = !settings.allowJavascript
                }
            }

            CheckboxButton {
                id: allowPopupCheckbox
                width: parent.width
                height: 50
                title.text: qsTr("Allow Popups")
                state: {
                    if(settings.allowPopup)
                        return "checked"
                    else return "unchecked"
                }
                onClicked :{
                    settings.allowPopup = !settings.allowPopup
                }
            }
            CheckboxButton {
                id: allowCookieCheckbox
                width: parent.width
                height: 50
                title.text: qsTr("Allow Cookies")
                state: {
                    if(settings.allowCookies)
                        return "checked"
                    else return "unchecked"
                }
                onClicked :{
                    settings.allowCookies = !settings.allowCookies
                }
            }
            CheckboxButton {
                id: allowImageCheckbox
                width: parent.width
                height: 50
                title.text: qsTr("Allow Images")
                state: {
                    if(settings.allowImages)
                        return "checked"
                    else return "unchecked"
                }
                onClicked :{
                    settings.allowImages = !settings.allowImages
                }
            }
        } // contentSettingColumn
    } // contentSettingComponent
}


