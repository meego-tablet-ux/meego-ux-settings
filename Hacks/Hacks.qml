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
        contentHeight: contents.height
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
                id: gpsSettings
                source: "image://theme/pulldown_box"
                width: parent.width

                Text {
                    id: textLabelgps
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.right: gpsEntry.left
                    anchors.leftMargin: 10
                    text: qsTr("Gps Device:")
                    width: 100
                    height: parent.height
                    verticalAlignment: Text.AlignVCenter
                }

                TextEntry {
                    id: gpsEntry
                    anchors.right: parent.right
                    width: parent.width / 2
                    text: gpsGconf.value
                    anchors.verticalCenter: parent.verticalCenter
                    onTextChanged: {
                        gpsGconf.value = gpsEntry.text
                    }

                    GConfItem {
                        id: gpsGconf
                        key: "/apps/geoclue/master/org.freedesktop.Geoclue.GPSDevice"
                        onValueChanged: {
                            gpsEntry.text = gpsGconf.value
                        }
                    }
                }

                /*Ux.Button {
                    id: selectBluetoothDevice
                    text: qsTr("Select Bluetooth Device")
                }*/
            }
        }
    }
}
