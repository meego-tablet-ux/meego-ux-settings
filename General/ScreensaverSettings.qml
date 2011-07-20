/*
* Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

import Qt 4.7
import MeeGo.Labs.Components 0.1 as Labs
import MeeGo.Ux.Components 0.1 as MeeGo
import MeeGo.Settings 0.1 as Settings

Item {
    id: screensaverItem
    width: parent.width
    height: childrenRect.height

    Settings.BacklightSetting {
        id: backlightSettings
    }

    Column{
        id: screensaverColumn
        anchors.left: parent.left
        width: parent.width
        spacing:  20
        Item {
            id: enabledRow
            width: parent.width
            height: enabledToggle.height
            Text {
                id: autoText
                anchors.left: parent.left
                anchors.leftMargin: 20
                anchors.right: enabledToggle.left
                anchors.rightMargin: 20
                text: qsTr("Screen Saver Enabled")
                font.pixelSize: theme.fontPixelSizeNormal
                color: theme.fontColorNormal
            }

            MeeGo.ToggleButton {
                id: enabledToggle
                anchors.right: parent.right
                anchors.rightMargin: 20
                on: backlightSettings.screenSaverTimeout > 0 ? true : false
                onToggled: {

                    if (!enabledToggle.on)
                        backlightSettings.screenSaverTimeout = 0
                    else
                        {
                        if (backlightSettings.screenSaverTimeout <= 0)
                            backlightSettings.screenSaverTimeout = 300

                        screensaverSlider.value = backlightSettings.screenSaverTimeout / 60                    
                    }
                }
            }
        }

        Item {
            width: parent.width
            height: sliderColumn.visible ? sliderColumn.height : 0
            Behavior on height {NumberAnimation {duration: 200}}
            Column {
                id: sliderColumn
                width: parent.width
                Text {
                    anchors.left: parent.left
                    anchors.leftMargin: 20
                    id: sliderText
                    text: qsTr("Screen Saver Timeout %n Minute(s)", "time in minutes", screensaverSlider.value)
                    width: sliderColumn.width
                    height: paintedHeight
                    font.pixelSize: theme.fontPixelSizeNormal
                    color: theme.fontColorNormal
                    wrapMode: Text.WordWrap
                    anchors.horizontalCenter: parent.horizontalCenter
                }
                MeeGo.Slider {
                    id: screensaverSlider                 
                    anchors.horizontalCenter: parent.horizontalCenter
                    width: parent.width
                    min: 1
                    max: 60
                    value: backlightSettings.screenSaverTimeout / 60
                    textOverlayVisible: false

                    onSliderChanged: {
                        backlightSettings.screenSaverTimeout = screensaverSlider.value * 60
                    }
                }

            }
            states: [
                State {
                    name: "visible"

                    PropertyChanges {
                        target: sliderColumn
                        opacity: 1.0
                    }

                    when: { enabledToggle.on == true }
                },

                State {
                    name: "hidden"

                    PropertyChanges {
                        target: sliderColumn
                        opacity: 0
                    }

                    when: { enabledToggle.on == false }
                }
            ]

            transitions: [
                Transition {
                    SequentialAnimation {
                        ScriptAction {
                            script: {
                                if (enabledToggle.on)
                                    sliderColumn.visible = true
                            }
                        }
                        NumberAnimation {
                            properties: "opacity"
                            duration: 350
                            easing.type: Easing.OutCubic
                        }
                        ScriptAction {
                            script: {
                                if (!enabledToggle.on)
                                    sliderColumn.visible = false
                            }
                        }
                    }
                }
            ]
        }
    }
}
