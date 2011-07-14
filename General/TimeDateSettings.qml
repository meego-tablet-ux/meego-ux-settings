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
import MeeGo.Ux.Components.DateTime 0.1
import MeeGo.Connman 0.1
import MeeGo.Settings 0.1

Item {
    id: page
    width: parent.width
    height: timeDateItem.height

    Labs.TimezoneListModel {
        id: timezoneListModel
    }

    Labs.LocaleHelper {
        id: locale
    }

    ClockModel {
        id: clockModel
    }

    Theme{
        id: theme
    }

    Column {
        id: timeDateItem
        width: parent.width

        Image {
            id: whatTimeisItLabel
            width: parent.width
            source: "image://themedimage/images/settings/subheader"

            Text{
                anchors.left: parent.left
                anchors.leftMargin: 20
                text: qsTr("What time it is");
                font.pixelSize: theme.fontPixelSizeNormal
                height: parent.height
                width: parent.width
                elide: Text.ElideRight
                verticalAlignment: Text.AlignVCenter
            }
        }

        ListSeparator {}
        Item {
            id: dateLabel
            height: theme_listBackgroundPixelHeightOne
            width: parent.width
            Text {
                id: dateLabelText
                text: locale.currentDate(Labs.LocaleHelper.DateFullLong)
                anchors.left: parent.left
                anchors.leftMargin: 20
                anchors.top: parent.top
                anchors.topMargin: 2
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: theme.fontPixelSizeNormal
                color: theme.fontColorNormal
                height: parent.height
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

        ListSeparator {}
        Item {
            id: timeLabel
            height: theme_listBackgroundPixelHeightOne
            width: parent.width

            Text {
                id: timeLabelText
                anchors.left: parent.left
                anchors.leftMargin: 20
                verticalAlignment: Text.AlignVCenter
                text: locale.currentTime(Labs.LocaleHelper.TimeFull)
                font.pixelSize: theme.fontPixelSizeNormal
                color: theme.fontColorNormal
                height: parent.height
                width: parent.width - 10
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    var coords = mapToItem(page.parent,mouseX,mouseY);
                    var date = new Date;
                    timePicker.hr24 = true
                    timePicker.hours = date.getHours();
                    timePicker.minutes = date.getMinutes();
                    timePicker.show(coords.x,coords.y);
                }
            }
        }

        ListSeparator {}
        Item {
            id: twelveHourToggle
            height: theme_listBackgroundPixelHeightOne
            width: parent.width

            Text {
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.leftMargin: 20
                anchors.right: twentyfourhrtoggle.left
                anchors.rightMargin: 20
                text: qsTr("24 hour clock")
                font.pixelSize: theme.fontPixelSizeNormal
                color: theme.fontColorNormal
                height: parent.height
                verticalAlignment: Text.AlignVCenter
            }

            ToggleButton {
                id: twentyfourhrtoggle
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 20
                on: locale.timeFormat == Labs.LocaleHelper.TimeFormat24
                onToggled: {
                    locale.timeFormat = twentyfourhrtoggle.on ? Labs.LocaleHelper.TimeFormat24 : Labs.LocaleHelper.TimeFormat12
                    timeTimer.interval = 1000
                }
            }
        }

        ListSeparator {}
        Item {
            id: automagicTimeToggle
            height: theme_listBackgroundPixelHeightOne
            width: parent.width

            Text {
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.leftMargin: 20
                anchors.right: autoTimeToggle.left
                anchors.rightMargin: 20
                text: qsTr("Set date and time automatically")
                wrapMode: Text.WordWrap
                height: parent.height
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: theme.fontPixelSizeNormal
                color: theme.fontColorNormal
            }

            ToggleButton {
                id: autoTimeToggle
                on: clockModel.timeUpdates == "auto"
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 20

                onToggled: {
                    if (on)
                        clockModel.timeUpdates = "auto"
                    else
                        clockModel.timeUpdates = "manual"
 }
		Connections {
                    target: clockModel
                    onTimeUpdatesChanged: {
                        autoTimeToggle.on = (clockModel.timeUpdates == "auto");
                    }
                }
            }
        }


        Image {
            id: whereAreYouLabel
            width: parent.width
            source: "image://themedimage/images/settings/subheader"

            Text{
                anchors.left: parent.left
                anchors.leftMargin: 20
                text: qsTr("Where you are");
                font.pixelSize: theme.fontPixelSizeNormal
                height: parent.height
                width: parent.width
                elide: Text.ElideRight
                verticalAlignment: Text.AlignVCenter
            }
        }

        Item {
            id: currentTz
            height: theme_listBackgroundPixelHeightOne
            width: parent.width

            Text {
                id: currentTzText
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.leftMargin: 20
                text: qsTr("Current time zone is %1").arg(timezoneListModel.getLocationName(clockModel.timezone))
                width: parent.width
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                font.pixelSize: theme.fontPixelSizeNormal
                color: theme.fontColorNormal
                height: parent.height
                verticalAlignment: Text.AlignVCenter
            }
        }

        ListSeparator {}
        Item {
            id: findMeToggle
            height: theme_listBackgroundPixelHeightOne
            width: parent.width

            Text {
                id: findMeText
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.leftMargin: 20
                anchors.right:  findMeToggleButton.left
                anchors.rightMargin: 20
                text: qsTr("Find me and keep my location updated")
                wrapMode: Text.WordWrap
                font.pixelSize: theme.fontPixelSizeNormal
                color: theme.fontColorNormal

                height: parent.height
                verticalAlignment: Text.AlignVCenter
            }

            ToggleButton {
                id: findMeToggleButton
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 20
                on: clockModel.timezoneUpdates == "auto"
                onToggled: {
                    if (on)
                        clockModel.timezoneUpdates = "auto";
                    else
                        clockModel.timezoneUpdates = "manual";
                }

                Connections {
                    target: clockModel
                    onTimezoneUpdatesChanged: {
                        findMeToggleButton.on = (clockModel.timezoneUpdates == "auto");
                    }
                }

            }
        }

        Image {
            id: manualTimezoneLabel
            width: parent.width
            source: "image://themedimage/images/settings/subheader"
            visible: !findMeToggleButton.on
            Text{
                id: manualTimezoneLabelText
                anchors.left: parent.left
                anchors.leftMargin: 20
                anchors.right: setTimeZoneButton.left
                anchors.rightMargin: 20
                text: qsTr("Manual time zone selection");
                font.pixelSize: theme.fontPixelSizeNormal
                color: theme.fontColorNormal
                height: parent.height
                wrapMode: Text.WordWrap
                verticalAlignment: Text.AlignVCenter
            }

            Button {
                id: setTimeZoneButton
                height: 50
                anchors.right: parent.right
                anchors.rightMargin: 20
                anchors.verticalCenter: parent.verticalCenter
                text: qsTr("Set Time Zone")

                onClicked: {
                    timezoneMenu.initAndShow(clockModel.timezone)
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
            dateLabelText.text = locale.currentDate(Labs.LocaleHelper.DateFullLong);
            timeLabelText.text = locale.currentTime(Labs.LocaleHelper.TimeFull);

            if(timeTimer.interval != 60000)
                timeTimer.interval = 60000
        }
    }

    TimePicker {
        id: timePicker

        onAccepted: {
            clockModel.timeUpdates = "manual"
            var time = clockModel.time(timePicker.hours, timePicker.minutes);
            clockModel.setTime(time);
            autoTimeToggle.on = (clockModel.timeUpdates == "auto")
            timeTimer.interval = 1000
        }
    }

    DatePicker{
        id: datePicker

        startYear: 1970
        onDateSelected: {
            clockModel.timeUpdates = "manual"
            clockModel.setDate(datePicker.selectedDate);
            autoTimeToggle.on = (clockModel.timeUpdates == "auto")
            timeTimer.interval = 1000
        }
    }

    SetTimezoneMenu {
        id: timezoneMenu

        onTriggered: {
            clockModel.timezone = newTzTitle;
            timeTimer.interval = 2000;
        }
    }
}
