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

        Grid {
            id: contents
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
