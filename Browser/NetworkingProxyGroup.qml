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
    id: proxyGroup
    title.text: qsTr("Networking Proxy")
    width: parent.width
    loader.sourceComponent: proxyComponent

    Component
    {
        id: proxyComponent

        Column {
            id: container
            width: parent.width
            height: childrenRect.height

            Connections {
                target: settings
                onDefaultRestored: {
                    noProxyRadioBtn.state = "checked";
                    proxyGroup.infoText = qsTr("No Proxy");
                    settings.useHttpProxy = false
                    useHttpProxyRadioBtn.state = "unchecked"
                }
                onUseHttpProxyChanged: {
                    if(settings.useHttpProxy) {
                        noProxyRadioBtn.state = "unchecked";
                        useHttpProxyRadioBtn.state = "checked";
                    }
                    else {
                        noProxyRadioBtn.state = "checked";
                        useHttpProxyRadioBtn.state = "unchecked";
                        proxyGroup.infoText = qsTr("No Proxy");
                    }
                }

            }
            RadioButton {
                id: noProxyRadioBtn
                width: parent.width
                title.text: qsTr("No Proxy")
                state: if(settings.useHttpProxy){
                    return "unchecked";
                } else {
                    proxyGroup.infoText = qsTr("No Proxy")
                    return "checked"
                }
                onClicked: {
                    noProxyRadioBtn.state = "checked"
                    useHttpProxyRadioBtn.state = "unchecked"
                    settings.useHttpProxy = false;
                }
            }

            RadioButton {
                id: useHttpProxyRadioBtn
                width: parent.width
                title.text: qsTr("Use Http Proxy")

                loader.sourceComponent: proxyInputComponent

                Component {
                    id: proxyInputComponent
                    Row {

                        id: inputBoxRow
                        spacing: 10
                        width: childrenRect.width
                        InputBox {
                            id: proxyBox
                            text: settings.proxyHost
                            width: 300
                            onTextChanged: {
                                if(textInput.focus) {
                                    settings.proxyHost = proxyBox.text
                                    if(portBox.text != "")
                                        settings.proxyPort = portBox.text
                                    else settings.proxyPort = 0
                                    settings.useHttpProxy = true;
                                    proxyGroup.infoText = proxyBox.text + ":" + portBox.text;
                                }
                            }
                        }
                        Text {
                            id: dotChar
                            text: ":"
                            color: "orange"
                            font.pixelSize: 32
                            width: 5
                        }

                        InputBox {
                            id: portBox
                            text: settings.proxyPort
                            width: 70
                            defaultText: "0"
                            textInput.validator: IntValidator { bottom: 0; top: 65535 }
                            onTextChanged: {
                                if(textInput.focus) {
                                    if(portBox.text != "")
                                        settings.proxyPort = portBox.text
                                    else settings.proxyPort = 0
                                    settings.proxyHost = proxyBox.text
                                    settings.useHttpProxy = true;
                                    proxyGroup.infoText = proxyBox.text + ":" + portBox.text;
                                }
                            }
                        }
                        Connections {
                            target: useHttpProxyRadioBtn
                            onClicked: {
                                if(portBox.text != "")
                                    settings.proxyPort = portBox.text
                                else settings.proxyPort = 0
                                settings.proxyHost = proxyBox.text
                                settings.useHttpProxy = true;
                                proxyGroup.infoText = proxyBox.text + ":" + portBox.text;
                            }
                        }
                    }
                }

                state: {
                    if(settings.useHttpProxy){
                        proxyGroup.infoText = settings.proxyHost + ":" + settings.proxyPort;
                        return "checked";
                    } else {
                        return "unchecked"
                    } 
                }
            }
        }
    } // proxyComponent
}


