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
    height: backlightColumn.height + 30

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
                height: paintedHeight
                wrapMode: Text.WordWrap
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

        Item {
            width: parent.width
            height: sliderColumn.visible ? sliderColumn.height : 0
            Behavior on height {NumberAnimation {duration: 200}}
            Column {
                id: sliderColumn
                width: parent.width

                visible: !autoToggle.on

                Item {
                    width: parent.width
                    height: sliderText.paintedHeight

                    Text {
                        id: sliderText
                        text: qsTr("Brightness")
                        font.pixelSize: theme_fontPixelSizeLarge
                        anchors.left: parent.left
                        anchors.leftMargin: 10
                        anchors.right: percentText.left
                        anchors.rightMargin: 10
                        elide: Text.ElideRight
                    }

                    Text {
                        id: percentText
                        text: qsTr("%1%","slider value percentage").arg(backlightSlider.value)
                        anchors.right: parent.right
                        anchors.rightMargin: 10
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
            }
            states: [
                State {
                    name: "visible"

                    PropertyChanges {
                        target: sliderColumn
                        opacity: 1.0
                    }

                    when: { autoToggle.on == false }
                },

                State {
                    name: "hidden"

                    PropertyChanges {
                        target: sliderColumn
                        opacity: 0
                    }

                    when: { autoToggle.on == true }
                }
            ]

            transitions: [
                Transition {
                    SequentialAnimation {
                        ScriptAction {
                            script: {
                                if (!autoToggle.on)
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
                                if (autoToggle.on)
                                    sliderColumn.visible = false
                            }
                        }
                    }
                }
            ]
        }
    }
}
