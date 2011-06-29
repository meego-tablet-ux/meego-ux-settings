/*
* Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

import Qt 4.7
import MeeGo.Labs.Components 0.1 as Labs
import MeeGo.Components 0.1 as MeeGo
import MeeGo.Settings 0.1 as Settings

Item {
    id: backlightItem
    width: parent.width
    height: childrenRect.height

    Settings.BacklightSetting {
        id: backlightSettings
    }

    Column{
        id: backlightColumn
        anchors.left: parent.left
        anchors.leftMargin: 10
        width: childrenRect.width
        spacing: 20

        Row {
            id: autoRow
            spacing: 20

            Text {
                id: autoText
                text: qsTr("Set brightness automatically")
		font.pixelSize: theme_fontPixelSizeLarge
            }

            MeeGo.ToggleButton {
                id: autoToggle
                on: backlightSettings.automatic
                onToggled: {
                    backlightSettings.automatic = autoToggle.on
                }
            }
        }

        Column {
            id: sliderColumn
            visible: !autoToggle.on
            height: childrenRect.height

            Item {
                width: childrenRect.width
                height: sliderText.paintedHeight

                Text {
                    id: sliderText
                    text: qsTr("Brightness")
                }

                Text {
                    text: qsTr("%1%","slider value percentage").arg(backlightSlider.value)
                    anchors.left: sliderText.right
                    anchors.leftMargin: 10
                }
            }

            Row {
                height: backlightSlider.height + 50

                Image {
                    source: "image://themedimage/widgets/common/brightness-slider/brightness-min"
                    anchors.verticalCenter: parent.verticalCenter
                }

                MeeGo.Slider {
                    id: backlightSlider
                    width: backlightItem.width / 2
                    value: backlightSettings.manualValue
                    textOverlayVisible: false

                    onSliderChanged: {
                        backlightSettings.manualValue = backlightSlider.value
                        backlightSlider.value = backlightSettings.manualValue
                    }
                }

                Image {
                    source: "image://themedimage/widgets/common/brightness-slider/brightness-max"
                    anchors.verticalCenter: parent.verticalCenter
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

                    when: { autoToggle.on == false }
                },

                State {
                    name: "hidden"

                    PropertyChanges {
                        target: sliderColumn
                        visible: false
                        height: 0
                        opacity: 0
                    }

                    when: { autoToggle.on == true }
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
