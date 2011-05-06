import Qt 4.7
import MeeGo.Labs.Components 0.1 as Labs
import MeeGo.Components 0.1 as MeeGo
import MeeGo.Settings 0.1

Labs.ApplicationPage {
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

                MeeGo.ToggleButton {
                    id: manualApn
                    on: true
                    anchors.right: parent.right
                    anchors.rightMargin: 10
                    anchors.verticalCenter: parent.verticalCenter
                }
            }

            Grid {
                id: dropDowns
                visible: !manualApn.on
                columns: 2
                spacing: 10
                width: parent.width
                height: childrenRect.height

                property string country
                property string provider: networkItem.name
                property string apn: cellularSettings.apn;

                Text {
                    text: qsTr("Country")
                }

                MeeGo.DropDown {
                    id: countryDropdown
                    model: cellularSettings.countries()
                    payload: cellularSettings.countries()
                    onTriggered: {
                        var data = payload[index]
                        country = data;
                        console.log("setting provider dropdownlist to: " + data)
                        providerDropdown.model = cellularSettings.providers(data)
                        providerDropdown.payload = cellularSettings.providers(data)
                    }
                }

                Text {
                    text: qsTr("Provider")
                }

                MeeGo.DropDown {
                    id: providerDropdown
                    model: cellularSettings.providers("")
                    //selectedIndex: cellularSettings.providers("").indexOf(provider)
                    onTriggered: {
                        provider = payload[index]
                        apnDropDown.model = cellularSettings.apns(country,provider)
                        apnDropDown.payload = cellularSettings.apns(country,provider)
                    }
                }

                Text {
                    text: qsTr("APN")
                }

                MeeGo.DropDown {
                    id: apnDropDown
                    model: cellularSettings.apns("","")
                    payload: cellularSettings.apns("","")
                    //selectedIndex: cellularSettings.apns("","").indexOf(apn)
                    onTriggered: {
                        apn = payload[index]
                        cellularSettings.setApn(apn)
                    }
                }
            }

            Grid {
                id: manualEntry
                width: parent.width
                columns: 2
                height: childrenRect.height
                visible: manualApn.on
                Text {
                    text: qsTr("APN")
                }

                MeeGo.TextEntry {
                    id: apn
                    width: parent.width / 3
                    text: cellularSettings.apn()

                }

                Text {
                    text: qsTr("Username")
                }

                MeeGo.TextEntry {
                    id: username
                    width: parent.width / 3
                    text: cellularSettings.username()

                }

                Text {
                    text: qsTr("Password")
                }

                MeeGo.TextEntry {
                    id: password
                    width: parent.width / 3
                    text: cellularSettings.password()
                }

                MeeGo.Button {
                    width: parent.width / 3
                    height: 50
                    text: qsTr("Apply");
                    elideText: true
                    onClicked: {
                        cellularSettings.setApn(apn.text, username.text, password.text)
                        networkItem.apn = apn.text
                    }
                }

            }
        }
    }
}
