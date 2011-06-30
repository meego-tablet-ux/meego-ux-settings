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
        anchors.leftMargin: 10
        width: parent.width
        spacing: 20

        Row {
            id: enabledRow
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 20

            Text {
                id: autoText
                text: qsTr("Screen Saver Enabled")
                font.pixelSize: theme_fontPixelSizeLarge
            }

            MeeGo.ToggleButton {
                id: enabledToggle
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

        Column {
            id: sliderColumn
            visible: enabledToggle.on
            width: parent.width

            Item {
                width: childrenRect.width
                height: sliderText.height
                anchors.horizontalCenter: parent.horizontalCenter

                Text {
                    id: sliderText
                    text: qsTr("Screen Saver Timeout")
                    font.pixelSize: theme_fontPixelSizeLarge
                }

                Text {
                    text: qsTr("%n Minute(s)","time in minutes",screensaverSlider.value)
                    font.pixelSize: theme_fontPixelSizeLarge
                    anchors.left: sliderText.right
                    anchors.leftMargin: 10
                }
            }

            MeeGo.Slider {
                id: screensaverSlider
                anchors.horizontalCenter: parent.horizontalCenter
                width: 400
                min: 1
                max: 60
                value: backlightSettings.screenSaverTimeout / 60
                textOverlayVisible: false

                onSliderChanged: {
                    backlightSettings.screenSaverTimeout = screensaverSlider.value * 60
                }
            }

            states: [
                State {
                    name: "visible"

                    PropertyChanges {
                        target: sliderColumn
                        height: childrenRect.height
                        visible: true
                        opacity: 1.0
                    }

                    when: { enabledToggle.on == true }
                },

                State {
                    name: "hidden"

                    PropertyChanges {
                        target: sliderColumn
                        visible: false
                        height: 0
                        opacity: 0
                    }

                    when: { enabledToggle.on == false }
                }
            ]

            transitions: [
                Transition {
                    SequentialAnimation {

                        NumberAnimation {
                            properties: "height"
                            duration: 200
                            easing.type: Easing.InCubic
                        }
                        NumberAnimation {
                            properties: "opacity"
                            duration: 350
                            easing.type: Easing.OutCubic
                        }
                    }
                }
            ]
        }
    }
}
