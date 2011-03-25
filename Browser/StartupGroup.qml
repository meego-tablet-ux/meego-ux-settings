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
    id: startupGroup
    title.text: qsTr("On Startup")
    width:parent.width
    loader.sourceComponent: startupComponent

    Component
    {
        id: startupComponent
        Column
        {
            id: startupColumn
            spacing:-5
            height: homepageRadioBtn.height + reopenRadioBtn.height - 5

            Connections {
                target: settings
                onPageOpenedOnStartupChanged: {
                    if(settings.pageOpenedOnStartup == BrowserSettingModel.OpenDefaultPages){
                        startupGroup.infoText = qsTr("Open home page");
                        homepageRadioBtn.state = "checked";
                        reopenRadioBtn.state = "unchecked"
                    } else {
                        homepageRadioBtn.state = "unchecked"
                        reopenRadioBtn.state = "checked"
                        startupGroup.infoText = qsTr("Reopen pages open last")
                    }
                }
            }

            RadioButton {
                id: homepageRadioBtn
                width: parent.width
                title.text: qsTr("Open the home page")
                state: {
                    if(settings.pageOpenedOnStartup == BrowserSettingModel.OpenDefaultPages){
                        startupGroup.infoText = qsTr("Open home page");
                        return "checked";
                    }
                    else return "unchecked";
                }
                onClicked: {
                    if(reopenRadioBtn.state == "checked")
                        reopenRadioBtn.state = "unchecked"
                    homepageRadioBtn.state = "checked"
                    startupGroup.infoText = qsTr("Open home page")
                    settings.pageOpenedOnStartup = BrowserSettingModel.OpenDefaultPages
                }
            }

            RadioButton {
                id: reopenRadioBtn
                width:parent.width
                title.text: qsTr("Reopen the pages that were open last")
                state: {
                    if(settings.pageOpenedOnStartup == BrowserSettingModel.OpenLastSessionPages){
                        startupGroup.infoText = qsTr("Reopen pages open last")
                        return "checked"
                    }
                    else return "unchecked"
                }

                onClicked: {
                    if(homepageRadioBtn.state == "checked")
                        homepageRadioBtn.state = "unchecked"
                    reopenRadioBtn.state = "checked"
                    startupGroup.infoText = qsTr("Reopen pages open last")
                    settings.pageOpenedOnStartup = BrowserSettingModel.OpenLastSessionPages
                }
            }
        } // startupColumn
    } // startupComponent
}


