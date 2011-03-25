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
    id: homepageGroup
    title.text: qsTr("Home Page")
    width: parent.width
    loader.sourceComponent: homepageComponent

    Component
    {
        id: homepageComponent
        Column
        {
            id: homepageColumn
            //anchors.fill:parent
            spacing:-5
            height: newtabRadioBtn.height + homepageRadioBtn.height - 5

            Connections {
                target: settings
                onNewTabIsHomePageChanged: {
                    if(settings.newTabIsHomePage){
                        homepageGroup.infoText = qsTr("New tab page")
                        newtabRadioBtn.state = "checked";
                        homepageRadioBtn.state = "unchecked"
                    }
                }
                onHomePageChanged: {
                    if(!settings.newTabIsHomePage) {
                        newtabRadioBtn.state = "unchecked";
                        homepageRadioBtn.state = "checked"
                        homepageGroup.infoText = settings.homePage;
                        settings.newTabIsHomePage = false;
                    }
                }
            }

            RadioButton {
                id: newtabRadioBtn
                width: parent.width
                title.text: qsTr("Use the new tab page as home page")
                state: {
                    if(settings.newTabIsHomePage){
                        homepageGroup.infoText = qsTr("New tab page");
                        return "checked";
                    }
                    else return "unchecked"
                }
                onClicked: {
                    if(homepageRadioBtn.state == "checked")
                        homepageRadioBtn.state = "unchecked"
                    newtabRadioBtn.state = "checked"
                    homepageGroup.infoText= qsTr("New tab page")
                    settings.newTabIsHomePage = true
                }
            }

            RadioButton {
                id: homepageRadioBtn
                width:parent.width
                title.text: qsTr("Open this page: ")
                state: {
                    if(!settings.newTabIsHomePage) {
                        homepageGroup.infoText = settings.homePage;
                        return "checked";
                    }
                    else return "unchecked";
                }

                loader.sourceComponent: inputArea
                Component {
                    id: inputArea
                    InputBox {
                        id: inputBox
                        text: settings.homePage
												width: 340
                        onTextChanged: {
                            settings.newTabIsHomePage = false
                            homepageGroup.infoText = inputBox.text
                            settings.homePage = inputBox.text
                            // Got key press in input field
                            newtabRadioBtn.state = "unchecked"
                            homepageRadioBtn.state = "checked"

                        }
                    }
                }
                onClicked: {
                    if(newtabRadioBtn.state == "checked")
                        newtabRadioBtn.state = "unchecked"
                    homepageRadioBtn.state = "checked"                    
                    homepageGroup.infoText = settings.homePage;
                    settings.newTabIsHomePage = false;
                    inputBox.text = settings.homePage;
                }
            }
        } // homepageColumn
    } // homepageComponent
}


