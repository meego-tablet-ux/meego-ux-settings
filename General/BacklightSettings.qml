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
    height: childrenRect.height + 30

    Settings.BacklightSetting {
        id: backlightSettings
    }

    Column{
        id: backlightColumn
        width: parent.width
        spacing: 20

        Item {
            id: autoRow
            width: parent.width
            height: theme_listBackgroundPixelHeightOne
            anchors.horizontalCenter: parent.horizontalCenter

            Text {
                id: autoText
                anchors.left: parent.left
                anchors.right: autoToggle.left
                anchors.rightMargin: 10
                text: qsTr("Set brightness automatically")
		font.pixelSize: theme_fontPixelSizeLarge
                anchors.verticalCenter: parent.verticalCenter
            }

            MeeGo.ToggleButton {
                id: autoToggle
                anchors.right: parent.right
                on: backlightSettings.automatic
                onToggled: {
                    backlightSettings.automatic = autoToggle.on
                }
                anchors.verticalCenter: parent.verticalCenter
            }
        }

        Column {
            id: sliderColumn
            anchors.left: parent.left
            anchors.right: parent.right

            visible: !autoToggle.on

            Item {
                width: parent.width
                height: sliderText.paintedHeight

                Text {
                    id: sliderText
                    text: qsTr("Brightness")
                    font.pixelSize: theme_fontPixelSizeLarge
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                Text {
                    text: qsTr("%1%","slider value percentage").arg(backlightSlider.value)
                    anchors.left: sliderText.right
                    anchors.leftMargin: 10
                }
            }

            Item {
                width: parent.width
                height: backlightSlider.height + 50

                Image {
                    id: minImage
                    source: "image://themedimage/widgets/common/brightness-slider/brightness-min"
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                }

                MeeGo.Slider {
                    id: backlightSlider
                    width: backlightItem.width / 2
                    value: backlightSettings.manualValue
                    anchors.left: minImage.right
                    anchors.right: maxImage.left
                    anchors.verticalCenter: parent.verticalCenter
                    textOverlayVisible: false

                    onSliderChanged: {
                        backlightSettings.manualValue = backlightSlider.value
                        backlightSlider.value = backlightSettings.manualValue
                    }
                }

                Image {
                    id: maxImage
                    source: "image://themedimage/widgets/common/brightness-slider/brightness-max"
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
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
