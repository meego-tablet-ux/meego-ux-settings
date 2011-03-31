import Qt 4.7
import MeeGo.Labs.Components 0.1
import MeeGo.Settings 0.1

ApplicationPage {
    id: container
    title: qsTr("Cellular Settings")

    property NetworkItemModel networkItem: null

    CellularSettingsModel { id: cellularSettings }

    Flickable {
        id: contentArea
        parent: container.content
        anchors.fill: parent
        clip: true
        contentWidth: parent.width
        contentHeight: contents.height

        Column {
            id: contents
            width: parent.width

            Image {
                id: bluetoothToggleGrid
                width: parent.width
                source: "image://theme/settings/pulldown_box_2"

                Text {
                    text: qsTr("Manual APN Entry")
                    width: 100
                    height: parent.height
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    verticalAlignment: Text.AlignVCenter
                }

                ToggleButton {
                    anchors.right: parent.right;
                    anchors.rightMargin: 10
                    anchors.verticalCenter: parent.verticalCenter
                }
            }

            Column {
                id: dropDowns
                width: parent.width
                property string country
                property string provider
                property string apn
                DropDown {
                    id: countryDropdown
                    dataModel: cellularSettings.countries();
                    onSelectionChanged: {
                        country = data;
                        providerDropdown.dataModel = cellularSettings.providers(data)
                    }
                }

                DropDown {
                    id: providerDropdown
                    selectedValue: provider
                    onSelectionChanged: {
                        provider = data
                        apnDropDown.dataModel = cellularSettings.apns(country,data)
                    }
                }

                DropDown {
                    id: apnDropDown
                    selectedValue: apn
                    onSelectionChanged: {
                        cellularSettings.setApn(apn)
                    }
                }
            }

            Grid {
                id: manualEntry
                width: parent.width
                columns: 2
                height: childrenRect.height

                Text {
                    text: qsTr("APN:")
                }

                TextEntry {
                    id: apn
                    width: parent.width / 3
                    text: cellularSettings.apn()

                }

                Text {
                    text: qsTr("Username:")
                }

                TextEntry {
                    id: username
                    width: parent.width / 3
                    text: cellularSettings.username()

                }

                Text {
                    text: qsTr("Password:")
                }

                TextEntry {
                    id: password
                    width: parent.width / 3
                    text: cellularSettings.password()
                }

                Button {
                    width: parent.width / 3
                    height: 50
                    title: qsTr("Apply");

                    onClicked: {
                        cellularSettings.setApn(apn.text, username.text, password.text)
                        networkItem.apn = apn.text
                    }
                }

            }
        }
    }
}
