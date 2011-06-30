/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

import Qt 4.7
import MeeGo.Labs.Components 0.1 as Labs
import MeeGo.Components 0.1

Item {
    id: container
    width: parent.width
    height: childrenRect.height + 30

    VolumeControl {
        id: volumeControl
        onVolumeChanged: {
            slider.value = volumeControl.volume
        }
    }


    Connections {
        target: slider
        onSliderChanged: {
            if(setVolumeTimer.running)
                setVolumeTimer.restart();
            else setVolumeTimer.start();
        }
    }

    Timer {
        id: setVolumeTimer
        interval: 250
        repeat:  false

        onTriggered: {
            volumeControl.volume = slider.value
            if(volumeControl.mute)
                volumeControl.mute = false;
        }
    }

    Item {
        height: childrenRect.height
        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.right: parent.right
        anchors.rightMargin: 20
        Image {
            id: volumeImage
            source: "image://themedimage/images/settings/icn_sound_low"
            anchors.left: parent.left
            anchors.verticalCenter: slider.verticalCenter

            property bool muted: volumeControl.mute

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    volumeControl.volume = 0
                }
            }
        }

        Slider {
            id: slider
            value: volumeControl.volume
            anchors.left: volumeImage.right
            anchors.leftMargin: 20
            anchors.right: volval.left
            anchors.rightMargin: 20
            anchors.verticalCenter: parent.verticalCenter
        }

        Image {
            id: volval
            anchors.right: parent.right
            anchors.verticalCenter: slider.verticalCenter
            source: "image://themedimage/images/settings/icn_sound_high"
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    volumeControl.volume = 100
                }
            }
        }

    }

}
