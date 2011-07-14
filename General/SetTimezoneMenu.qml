import Qt 4.7
import MeeGo.Labs.Components 0.1 as Labs
import MeeGo.Ux.Components.Common 0.1

ModalDialog {
    id: dialog

    height: parent.height - 150

    title : qsTr("Set Time Zone")

    showAcceptButton: true
    showCancelButton: true
    acceptButtonEnabled: timezoneList.currentItem != undefined

    property bool embedded: false
    property bool landscape: true
    property variant model: undefined
    property int itemHeight: 45

    signal triggered(string newTzTitle)
    signal close()

    function initAndShow(title) {
        locEntry.text = ""
        timezoneList.selectTitle(title);
        dialog.show();
    }

    function handleAccept() {
        dialog.triggered(timezoneList.currentItem.selectedtitle);
        dialog.close();
    }

    onAccepted: handleAccept()
    onRejected: close()

    content: Item {
        anchors.fill: parent
        anchors.margins: 20

        Text {
            id: locLabel
            anchors { top: parent.top; left: parent.left }
            color: theme_fontColorMedium
            font.pixelSize: 16
            text: qsTr("Choose location:")
        }
        TextEntry {
            id: locEntry
            anchors { top: locLabel.bottom; left: parent.left; right: parent.right }
            anchors.topMargin: 10
            font.pixelSize: 18
            onTextChanged: timezoneList.filter(text)
        }
        Labs.TimezoneList {
            id: timezoneList
            anchors { top: locEntry.bottom; left: parent.left; right: parent.right; bottom: parent.bottom }
            anchors.topMargin: 10
        }
    }
}
