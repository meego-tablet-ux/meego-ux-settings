import Qt 4.7
/*import MeeGo.Ux.Components.Common 0.1
import MeeGo.Ux.Components.Indicators 0.1*/
import MeeGo.Components 0.1
import MeeGo.Settings 0.1

AppPage {
    id: page
    pageTitle: qsTr("General")

    VolumeControl {
        id: volumeControl
    }

    Column {
        id: contents
        width:  parent.width

        ExpandingBox {
            id: timedateexpandingbox
            property int containerHeight: 80
            height: containerHeight
            anchors.margins: 20
            anchors.left: parent.left
            anchors.right: parent.right

            detailsComponent: TimeDateSettings { }

            Text {
                text: qsTr ("Time and date")
                height: timedateexpandingbox.containerHeight
                verticalAlignment: Text.AlignVCenter
            }
        }

        ExpandingBox {
            id: volumeexpandingbox
            property int containerHeight: 80
            height: containerHeight

            anchors.margins: 20
            anchors.left: parent.left
            anchors.right: parent.right

            Text {
                text: qsTr ("Sound")
                height: volumeexpandingbox.containerHeight
                verticalAlignment: Text.AlignVCenter
            }

            Text {
                // %1 is volume level percentage
                text: qsTr("%1%","%1 is volume level percentage").arg(volumeControl.volume)
                height: volumeexpandingbox.containerHeight
                anchors.right: parent.right
                anchors.rightMargin: 40
                verticalAlignment: Text.AlignVCenter
            }

            detailsComponent: VolumeSettings { }
        }

        ExpandingBox {
            id: backlightexpandingbox
            property int containerHeight: 80
            height: containerHeight

            anchors.margins: 20
            anchors.left: parent.left
            anchors.right: parent.right

            Text {
                text: qsTr ("Backlight control")
                height: backlightexpandingbox.containerHeight
                verticalAlignment: Text.AlignVCenter
            }

            detailsComponent: BacklightSettings { }
        }

        ExpandingBox {
            id: screensaverexpandingbox
            property int containerHeight: 80
            height: containerHeight

            anchors.margins: 20
            anchors.left: parent.left
            anchors.right: parent.right

            Text {
                text: qsTr ("Screen saver")
                height: screensaverexpandingbox.containerHeight
                verticalAlignment: Text.AlignVCenter
            }

            detailsComponent: ScreensaverSettings { }
        }

    }

}
