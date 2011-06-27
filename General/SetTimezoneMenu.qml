import Qt 4.7
import MeeGo.Labs.Components 0.1 as Labs
import MeeGo.Ux.Components.Common 0.1

ModalDialog {
    id: dialog

    height: parent.height - 150

    title : qsTr("Set Time Zone")

    showAcceptButton: true
    showCancelButton: true
    acceptButtonEnabled: (tzlistmodel.currentItem != undefined)&&(searchBar.text != "")

    property bool embedded: false
    property bool landscape: true
    property variant model: undefined
    property int itemHeight: 45

    signal triggered(string newTzTitle)
    signal close()

    function initAndShow(title) {

        if (title != undefined) {
            timezonelist.filterOut(title);
            tzlistmodel.currentIndex = 0;
            searchBar.text = title;
        }
        else {
            tzlistmodel.currentIndex = -1;
            searchBar.text = "";
        }

        dialog.show();
    }

    function handleAccept() {
        if((tzlistmodel.currentItem != undefined)&&(searchBar.text != ""))
        {
            dialog.triggered(tzlistmodel.currentItem.tzTitle);
            dialog.close();
        }
    }

    onAccepted: handleAccept()
    onRejected: close()

    content: Item {
        id: container
        anchors.fill:parent
        anchors.margins: 20

        Labs.TimezoneListModel {
            id: timezonelist
        }

        Text {
            id: filterTitle
            anchors.top: parent.top
            anchors.left: parent.left
            height:  55
            verticalAlignment: Text.AlignVCenter
            text: qsTr("Filter list")
            font.pixelSize: theme_fontPixelSizeLarge
        }

        TextEntry {
            id: searchBar
            anchors.top: parent.top
            anchors.left: filterTitle.right
            anchors.leftMargin: 10
            anchors.right: parent.right

            width: parent.width - filterTitle.paintedWidth
            height: 55

            onTextChanged: {
                timezonelist.filterOut(searchBar.text);
                tzlistmodel.currentIndex = 0;
            }
            Keys.onReturnPressed: {
                dialog.handleAccept()
            }
        }

        Image {
            id: timezones
            anchors.top: searchBar.bottom
            anchors.topMargin: 10
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right

            source: "image://themedimage/images/clock/bg_grooved_area"
            ListView {
                id: tzlistmodel
                anchors.fill: parent
                clip: true
                z: -1
                model: timezonelist

                delegate: Image {
                    id: timerect
                    property int gmt: gmtoffset
                    property string tzTitle: title
                    source: "image://themedimage/images/clock/bg_list_item"
                    height: itemHeight
                    width: parent.width

                    Text {
                        text: locationname
                        anchors.left: timerect.left
                        anchors.leftMargin: 15
                        anchors.verticalCenter: parent.verticalCenter
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        wrapMode: Text.WordWrap
                        font.pixelSize: theme_fontPixelSizeLarge
                    }
                    /*Text {
                        text: (gmtoffset < 0) ? qsTr("(GMT %1%2)").arg(gmtoffset).arg(":00") : qsTr("(GMT +%1%2)").arg(gmtoffset).arg(":00")
                        anchors.right: timerect.right
                        anchors.rightMargin: 15
                        anchors.verticalCenter: parent.verticalCenter
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        wrapMode: Text.WordWrap
                        font.pixelSize: theme_fontPixelSizeLarge
                    }*/
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            tzlistmodel.currentIndex = index;
                            searchBar.text = locationname;
                        }
                    }
                }
            }
        }
    }
}
