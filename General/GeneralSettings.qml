import Qt 4.7
/*import MeeGo.Ux.Components.Common 0.1
import MeeGo.Ux.Components.Indicators 0.1*/
import MeeGo.Components 0.1
import MeeGo.Settings 0.1

AppPage {
    id: page
    pageTitle: qsTr("General")
    height: contents.height + 10

    VolumeControl {
        id: volumeControl
    }
    Theme {
        id: theme
    }
    Column {
        id: contents
        width:  parent.width

        ExpandingBox {
            id: languageexpandingbox
            property int containerHeight: 80
            height: containerHeight
            anchors.left: parent.left
            anchors.right: parent.right

            detailsComponent: LanguageSettings { }

            Text {
                text: qsTr ("Language")
                anchors.left: parent.left
                anchors.leftMargin: 20
                anchors.right:  parent.right
                anchors.rightMargin: 40
                verticalAlignment: Text.AlignVCenter
                height:  parent.containerHeight
                font.pixelSize: theme.fontPixelSizeNormal
            }
        }

        Image {
            id: timedateexpandingbox
            height: theme_listBackgroundPixelHeightOne
            source: "image://themedimage/images/settings/subheader"
            width: parent.width
            Text {
                id: timeTitle
                anchors.left: parent.left
                anchors.leftMargin: 20
                text: qsTr ("Time and date")
                font.pixelSize: theme.fontPixelSizeNormal
                elide: Text.ElideRight
                anchors.right: btnIcon.left
                anchors.rightMargin: 10
                height: parent.height
                verticalAlignment: Text.AlignVCenter
            }
            Image {
                id: btnIcon
                anchors.right: parent.right
                anchors.rightMargin: 20
                anchors.verticalCenter: parent.verticalCenter
                source: "image://themedimage/icons/internal/arrow-default-right"
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    window.addPage(timeSettings)
                }
            }
            Component {
                id: timeSettings
                AppPage {
                    pageTitle: timeTitle.text
                    height: timedateContent.height
                    TimeDateSettings {
                        id: timedateContent
                    }
                }
            }
        }

        //ListSeparator {}
        Image {
            id: sound
            width: parent.width
            height: theme.listBackgroundPixelHeightOne
            source: "image://themedimage/images/settings/subheader"
            Text {
                text: qsTr ("Sound")
                font.pixelSize: theme.fontPixelSizeNormal
                elide: Text.ElideRight
                anchors.left: parent.left
                anchors.leftMargin: 20
                anchors.right: volPercentage.left
                anchors.rightMargin: 5
                anchors.verticalCenter: parent.verticalCenter
            }
            Text {
                id: volPercentage
                // %1 is volume level percentage
                text: qsTr("%1%","%1 is volume level percentage").arg(volumeControl.volume)
                font.pixelSize: theme.fontPixelSizeNormal
                color: theme.fontColorNormal
                anchors.right: parent.right
                anchors.rightMargin: 20
                anchors.verticalCenter: parent.verticalCenter
            }
        }
        Item {
            width: parent.width
            height: vol.height + 40
            VolumeSettings {
                id: vol
                anchors.verticalCenter: parent.verticalCenter
            }
        }




       // ListSeparator {}
        Image {
            id: backlightexpandingbox
            height: theme.listBackgroundPixelHeightOne
            source: "image://themedimage/images/settings/subheader"
            width: parent.width

            Text {
                id: backlightTitle
                height: theme.listBackgroundPixelHeightOne
                text: qsTr ("Backlight control")
                font.pixelSize: theme.fontPixelSizeNormal
                elide: Text.ElideRight
                anchors.left: parent.left
                anchors.leftMargin: 20
                verticalAlignment: Text.AlignVCenter
            }
        }
        Item {
            width: parent.width
            height: backlight.height + 40
            BacklightSettings {
                id: backlight
                anchors.verticalCenter: parent.verticalCenter
            }
        }


        ///ListSeparator {}
        Image {
            id: screensaverexpandingbox
            height: theme.listBackgroundPixelHeightOne
            source: "image://themedimage/images/settings/subheader"

            width: parent.width
            Text {
                id: screensaverTitle
                height: theme_listBackgroundPixelHeightOne
                text: qsTr ("Screen saver")
                font.pixelSize: theme.fontPixelSizeNormal
                elide: Text.ElideRight
                anchors.left: parent.left
                anchors.leftMargin: 20
                verticalAlignment: Text.AlignVCenter
            }
        }
        Item {
            width: parent.width
            height: screensaver.height + 40
            ScreensaverSettings {
                id: screensaver
                anchors.verticalCenter: parent.verticalCenter
            }
        }
    }
}
