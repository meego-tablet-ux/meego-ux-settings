/*
* Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

import Qt 4.7
import MeeGo.Labs.Components 0.1
import MeeGo.Components 0.1 as MeeGo
import MeeGo.Settings 0.1

ApplicationPage {
    id: page
    title: qsTr("Time and Date")

    TimezoneListModel {
        id: timezoneListModel
    }

    Item {
        anchors.fill: page.content

        Flickable {
            contentHeight: timeDateItem.height
            anchors.fill: parent
            clip: true
            Column {
                id: timeDateItem
                width: parent.width

                Image {
                    id: whatTimeisItLabel
                    width: parent.width
                    source: "image://theme/settings/subheader"

                    Text{
                        anchors.left: parent.left
                        anchors.leftMargin: 10
                        text: qsTr("What time it is");
                        font.pixelSize: theme_fontPixelSizeLarge
                        height: parent.height
                        width: parent.width
                        elide: Text.ElideRight
                        verticalAlignment: Text.AlignVCenter
                    }
                }

                Image {
                    id: dateLabel
                    source: "image://theme/pulldown_box"
                    width: parent.width
                    Text {
                        id: dateLabelText
                        text: timeSettings.currentDate()
                        anchors.left: parent.left
                        anchors.leftMargin: 10
                        anchors.top: parent.top
                        anchors.topMargin: 2
                        font.pixelSize: theme_fontPixelSizeLargest3
                        height: 100
                        width: parent.width
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            var coords = mapToItem(page.parent,mouseX,mouseY)
                            datePicker.show(coords.x,coords.y);
                        }
                    }
                }

                Image {
                    id: timeLabel
                    source: "image://theme/pulldown_box"
                    width: parent.width

                    Text {
                        id: timeLabelText
                        anchors.left: parent.left
                        anchors.leftMargin: 10
                        verticalAlignment: Text.AlignVCenter
                        text: timeSettings.currentTime()
                        font.pixelSize: theme_fontPixelSizeLargest3
                        height: parent.height
                        width: parent.width - 10
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            var coords = mapToItem(page.parent,mouseX,mouseY)
                            timePicker.show(coords.x,coords.y);
                        }
                    }
                }

                Image {
                    id: twelveHourToggle
                    source: "image://theme/pulldown_box"
                    width: parent.width

                    Text {
                        anchors.top: parent.top
                        anchors.left: parent.left
                        anchors.leftMargin: 10
                        text: qsTr("24 hour clock")
                        width: 100
                        height: parent.height
                        verticalAlignment: Text.AlignVCenter
                    }

                    MeeGo.ToggleButton {
                        id: twentyfourhrtoggle
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.right: parent.right
                        anchors.rightMargin: 10
                        on: timeSettings.flag24
                        onToggled: {
                            timeSettings.flag24 = twentyfourhrtoggle.on
                            //twentyfourhrtoggle.on = timeSettings.flag24
                        }
                    }
                }

                Image {
                    id: automagicTimeToggle
                    source: "image://theme/pulldown_box"
                    width: parent.width

                    Text {
                        anchors.top: parent.top
                        anchors.left: parent.left
                        anchors.leftMargin: 10
                        text: qsTr("Set date and time automatically")
                        width: 100
                        height: parent.height
                        verticalAlignment: Text.AlignVCenter
                    }

                    MeeGo.ToggleButton {
                        on: timeSettings.automatic
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.right: parent.right
                        anchors.rightMargin: 10

                        onToggled: {
                            timeSettings.automatic = isOn;
                        }
                    }

                }


                Image {
                    id: whereAreYouLabel
                    width: parent.width
                    source: "image://theme/settings/subheader"

                    Text{
                        anchors.left: parent.left
                        anchors.leftMargin: 10
                        text: qsTr("Where you are");
                        font.pixelSize: theme_fontPixelSizeLarge
                        height: parent.height
                        width: parent.width
                        elide: Text.ElideRight
                        verticalAlignment: Text.AlignVCenter
                    }
                }

                Image {
                    id: currentTz
                    source: "image://theme/pulldown_box"
                    width: parent.width

                    Text {
                        id: currentTzText
                        anchors.top: parent.top
                        anchors.left: parent.left
                        anchors.leftMargin: 10
                        text: qsTr("Current Timezone is %1").arg(timeSettings.getHumanReadableTz())
                        width: parent.width
                        height: parent.height
                        verticalAlignment: Text.AlignVCenter
                    }
                }

                Image {
                    id: findMeToggle
                    source: "image://theme/pulldown_box"
                    width: parent.width

                    Text {
                        id: findMeText
                        anchors.top: parent.top
                        anchors.left: parent.left
                        anchors.leftMargin: 10
                        text: qsTr("Find me and keep my location updated")
                        width: parent.width
                        height: parent.height
                        verticalAlignment: Text.AlignVCenter
                    }

                    MeeGo.ToggleButton {
                        id: findMeToggleButton
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.right: parent.right
                        anchors.rightMargin: 10
                        on: timeSettings.isUsingTzAuto()
                        onToggled: {
                            timeSettings.setTzAuto(isOn);
                            currentTzText.text = qsTr("Current Timezone is ") + timeSettings.getHumanReadableTz();
                        }
                    }
                }

                Image {
                    id: manualTimezoneLabel
                    width: parent.width
                    source: "image://theme/settings/subheader"
                    // visible: !findMeToggleButton.on
                    Text{
                        id: manualTimezoneLabelText
                        anchors.left: parent.left
                        anchors.leftMargin: 10
                        text: qsTr("Manual time zone selection");
                        font.pixelSize: theme_fontPixelSizeLargest
                        font.bold: true
                        height: parent.height
                        width: parent.width
                        elide: Text.ElideRight
                        verticalAlignment: Text.AlignVCenter
                    }

                    MeeGo.Button {
                        id: setTimeZoneButton
                        height: 40
                        width: 180
                        anchors.right: parent.right
                        anchors.rightMargin: 10
                        anchors.verticalCenter: parent.verticalCenter
                        //active: !findMeToggleButton.on
                        bgSourceUp: "image://theme/btn_blue_up"
                        bgSourceDn: "image://theme/btn_blue_dn"
                        title: qsTr("Set Time Zone")
                        //font.pixelSize: theme_fontSizeLargest
                        color: theme_buttonFontColor
                        //visible: !findMeToggleButton.on

                        onClicked: {
                            timezoneSelectLoader.source = "SetTimezoneMenu.qml"
                        }

                        Connections {
                            target: timezoneSelectLoader.item
                            onTriggered: {
                                var saveSuccess = timeSettings.setTz(newTzTitle);
                                if (saveSuccess)
                                {
                                    currentTzText.text = qsTr("Current Timezone is ") + timeSettings.getHumanReadableTz();
                                    timeTimer.interval = 2000
                                }

                                else
                                    manualTimezoneLabelText.text = qsTr("Unable to set timezone manually");
                            }

                            onClose: {
                                timezoneSelectLoader.sourceComponent = undefined
                            }
                        }
                    }
                }

            }
        }

        Timer {
            id: timeTimer
            interval: 60000
            running: true
            repeat: true
            onTriggered: {
                dateLabelText.text = timeSettings.currentDate()
                timeLabelText.text = timeSettings.currentTime();
                if(timeTimer.interval != 60000)
                    timeTimer.interval = 60000
            }
        }

        MeeGo.TimePicker {
            id: timePicker

            onAccepted: {
                var time = timeSettings.time(timePicker.hours, timePicker.minutes, "00")
                timeSettings.setTime(time);
                timeTimer.interval = 1000
            }
        }

        MeeGo.DatePicker{
            id: datePicker

            onDateSelected: {
                timeSettings.setDate(date);
                timeTimer.interval = 1000
            }
        }

		TimeSettings {
			id: timeSettings
        }

		///I hate using loaders
		Loader{
			id: timezoneSelectLoader
			anchors.fill: parent
			width: parent.width
			height: parent.height
		}
    }
}
