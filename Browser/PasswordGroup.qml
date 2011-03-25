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
    id: passwordGroup
    title.text: qsTr("Passwords")
    width:parent.width
    loader.sourceComponent: passwordComponent

    Component
    {
        id: passwordComponent
        Column
        {
            id: passwordColumn
            //anchors.fill:parent
            spacing:-5
            height: oftensaveRadioBtn.height + neversaveRadioBtn.height - 5

            Connections {
                target: settings
                onSavePasswordChanged: {
                    if(settings.savePassword) {
                        neversaveRadioBtn.state = "unchecked"
                        oftensaveRadioBtn.state = "checked"
                        passwordGroup.infoText = qsTr("Offer to save passwords")
                    } else {
                        neversaveRadioBtn.state = "checked"
                        oftensaveRadioBtn.state = "unchecked"
                        passwordGroup.infoText = qsTr("Never save passwords")
                    }
                }
            }

            RadioButton {
                id: oftensaveRadioBtn
                width: parent.width
                title.text: qsTr("Offer to save passwords")
                state: {
                    if(settings.savePassword) {
                        passwordGroup.infoText = qsTr("Offer to save passwords")
                        return "checked";
                    } else {
                        passwordGroup.infoText = qsTr("Never save passwords")
                        return "unchecked"
                    }
                }
                onClicked: {
                    neversaveRadioBtn.state = "unchecked"
                    oftensaveRadioBtn.state = "checked"
                    passwordGroup.infoText = qsTr("Offer to save passwords")
                    settings.savePassword = true;
                }
            }

            RadioButton {
                id: neversaveRadioBtn
                width:parent.width
                title.text: qsTr("Never save passwords")
                state: {
                    if(!settings.savePassword) {
                        passwordGroup.infoText = qsTr("Never save passwords")
                        return "checked";
                    } else {
                        passwordGroup.infoText = qsTr("Offer to save passwords")
                        return "unchecked"
                    }
                }
                onClicked: {
                    neversaveRadioBtn.state = "checked"
                    oftensaveRadioBtn.state = "unchecked"
                    passwordGroup.infoText = qsTr("Never save passwords")
                    settings.savePassword = false;
                }
            }
        } // passwordColumn
    } // passwordComponent
}


