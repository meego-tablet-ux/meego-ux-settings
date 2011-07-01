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

            titleText: qsTr ("Language")              
        }

        Item {
            id: timedateexpandingbox
            height: theme_listBackgroundPixelHeightOne
            anchors.margins: 10
            anchors.left: parent.left
            anchors.right: parent.right
            Text {
                id: timeTitle
                text: qsTr ("Time and date")
                font.pixelSize: theme.fontPixelSizeLarge
                color: theme.fontColorHighlight
                elide: Text.ElideRight
                anchors.left: parent.left
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
                source: "image://themedimage/icons/internal/arrow-default-right.png"
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

        ListSeparator {}
        Item {
            id: volumeexpandingbox

            anchors.margins: 10
            anchors.left: parent.left
            anchors.right: parent.right

            height: sound.height + vol.height
            Item {
                id: sound
                width: parent.width
                height: theme_listBackgroundPixelHeightOne
                Text {
                    text: qsTr ("Sound")
                    font.pixelSize: theme.fontPixelSizeLarge
                    color: theme.fontColorHighlight
                    elide: Text.ElideRight
                    anchors.left: parent.left
                    anchors.right: volPercentage.left
                    anchors.rightMargin: 5
                    anchors.verticalCenter: parent.verticalCenter
                }
                Text {
                    id: volPercentage
                    // %1 is volume level percentage
                    text: qsTr("%1%","%1 is volume level percentage").arg(volumeControl.volume)
                    height: volumeexpandingbox.containerHeight
                    anchors.right: parent.right
                    anchors.rightMargin: 40
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
            VolumeSettings {
                id: vol
                anchors.top: sound.bottom
            }
        }

        ListSeparator {}
        Item {
            id: backlightexpandingbox
            height: backlightTitle.height + backlight.height

            anchors.margins: 10
            anchors.left: parent.left
            anchors.right: parent.right

            Text {
                id: backlightTitle
                height: theme_listBackgroundPixelHeightOne
                text: qsTr ("Backlight control")
                font.pixelSize: theme.fontPixelSizeLarge
                color: theme.fontColorHighlight
                elide: Text.ElideRight
                anchors.left: parent.left
                verticalAlignment: Text.AlignVCenter
            }
            BacklightSettings {
                id: backlight
                anchors.top: backlightTitle.bottom
            }
        }

        ListSeparator {}
        Item {
            id: screensaverexpandingbox
            height: screensaverTitle.height + screensaver.height

            anchors.margins: 10
            anchors.left: parent.left
            anchors.right: parent.right
            Text {
                id: screensaverTitle
                height: theme_listBackgroundPixelHeightOne
                text: qsTr ("Screen saver")
                font.pixelSize: theme.fontPixelSizeLarge
                color: theme.fontColorHighlight
                elide: Text.ElideRight
                anchors.left: parent.left
                verticalAlignment: Text.AlignVCenter
            }

            ScreensaverSettings {
                id: screensaver
                anchors.top: screensaverTitle.bottom
            }

        }

    }

}
