import Qt 4.7
import MeeGo.Settings 0.1
import MeeGo.Labs.Components 0.1
import MeeGo.Components 0.1 as Ux

ApplicationPage {
    id: page
    title:  qsTr("Devloper Hacks")
    //default property alias pagechildren: page.content

    Flickable {
        parent:  page.content
        contentHeight: contents.height + 500
        anchors.fill: parent
        clip: true

        Column {
            id: contents
            width: parent.width

            Image {
                id: languageItem
                source: "image://theme/pulldown_box"
                width: parent.width

                Text {
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    text: qsTr("Optimal hacker defaults:")
                    width: 100
                    height: parent.height
                    verticalAlignment: Text.AlignVCenter
                }

                Ux.ToggleButton {
                    id: optimalToggle
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    anchors.rightMargin: 10
                }
            }

            Image {
                id: notificationMakerBar
                source: "image://theme/settings/subheader"
                width: parent.width

                Ux.ExpandingBox {
                    id: expandingBox

                    width: parent.width
                    titleText: "Notification Maker"
                    titleTextColor: "black"
                    anchors.centerIn:  parent
                    detailsComponent: notificationMakerComponent

                    Component {
                        id: notificationMakerComponent

                        Column{
                            width: parent.width
                            spacing: 10
                            Image {
                                id: notificationItem
                                source: "image://theme/settings/subheader"
                                width: parent.width
                                height: 60

                                Ux.DropDown{
                                    id: typeDropdown
                                    property string eventTypeText: "x-nokia.message"
                                    width: parent.width - 50
                                    title: "Select Notification Event Type"
                                    anchors.centerIn:  parent
                                    replaceDropDownTitle: true
                                    titleColor: "black"

                                    model: [
                                        notificationModel.get(0).name,
                                        notificationModel.get(1).name,
                                        notificationModel.get(2).name,
                                        notificationModel.get(3).name,
                                        notificationModel.get(4).name,
                                        notificationModel.get(5).name,
                                        notificationModel.get(6).name,
                                        notificationModel.get(7).name,
                                        notificationModel.get(8).name,
                                        notificationModel.get(9).name,
                                        notificationModel.get(10).name,
                                        notificationModel.get(11).name
                                    ]

                                    onTriggered: { eventTypeText = notificationModel.get(index).eventType }
                                }
                            }

                            Image {
                                id: summaryItem
                                source: "image://theme/settings/subheader"
                                width: parent.width
                                height: 50

                                Row{
                                    anchors.verticalCenter:  parent.verticalCenter
                                    Text {
                                        id: summaryText
                                        text: "Enter Notification Summary: "
                                    }

                                    TextEdit{
                                        id:summaryInput
                                        width: 500
                                        cursorVisible: true
                                        font.underline: true
                                        font.weight: Font.DemiBold
                                        color: "blue"
                                        text: "Summary"
                                    }
                                }
                            }
                            Image {
                                id: bodyItem
                                source: "image://theme/settings/subheader"
                                width: parent.width
                                height: 50

                                Row{
                                    anchors.verticalCenter:  parent.verticalCenter
                                    Text {
                                        id: bodyText
                                        text: "Enter Notification Summary: "
                                    }

                                    TextEdit{
                                        id:bodyInput
                                        width: 500
                                        cursorVisible: true
                                        font.underline: true
                                        font.weight: Font.DemiBold
                                        color: "blue"
                                        text: "Body"
                                    }
                                }
                            }

                            Image {
                                id: imageItem
                                source: "image://theme/settings/subheader"
                                width: parent.width
                                height: 50

                                Row{
                                    anchors.verticalCenter:  parent.verticalCenter
                                    Text {
                                        id: imageText
                                        text: "Enter Notification Summary: "
                                    }

                                    TextEdit{
                                        id:imageInput
                                        width: 500
                                        cursorVisible: true
                                        font.underline: true
                                        font.weight: Font.DemiBold
                                        color: "blue"
                                        text: "image://meegotheme/icons/settings/everyday-settings"
                                    }
                                }
                            }

                            Ux.Button{
                                id: sendNotificationButton
                                text: "Make Notification"
                                height: 40
                                anchors.topMargin: 10
                                bgSourceUp: "image://theme/btn_blue_up"
                                bgSourceDn: "image://theme/btn_blue_dn"
                                onClicked: {
                                    notificationMaker.makeNotification(typeDropdown.eventTypeText, summaryInput.text, bodyInput.text, imageInput.text);

                                }
                            }
                        }
                    }
                }
            }
        }
    }


    NotificationMakerModel {
        id: notificationModel
    }

    NotificationMaker {
        id: notificationMaker
    }

}

