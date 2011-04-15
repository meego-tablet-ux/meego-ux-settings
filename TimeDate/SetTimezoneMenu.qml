import Qt 4.7
import MeeGo.Labs.Components 0.1

Item {
    id: container
    anchors.fill:parent

    property alias menuWidth: menu.width
    property alias menuHeight: menu.height
    property variant model: undefined

    property alias menuOpacity: menu.opacity

    property int itemHeight: 50
    property bool embedded: false
    property bool landscape: true

    signal triggered(string c_name, string newTzTitle, int c_gmt)
    signal close()

    function initialize(title, gmt) {
        timezonelist.filterOut(title);
        tzlistmodel.currentIndex = 0;
        tzlistmodel.highlight = highlighter;
        inputElement.text = title;
    }

    TimezoneListModel {
        id: timezonelist
    }

    Component {
        id: highlighter	
        Rectangle {
            color: "green"
            width: 200
            height: 40
        }
    }

    Component {
        id: highlighteroff
        Rectangle {
            color: "transparent"
        }
    }

    Rectangle {
        id: fog
        anchors.fill: parent
        color: theme_dialogFogColor
        opacity: theme_dialogFogOpacity
    }

    MouseArea {
        anchors.fill: parent
        onClicked: container.close()

        Image {
            id: menu
            width:parent.width - 100
            height: parent.height -100
            source: "image://theme/bg_application_p"
            anchors.centerIn: parent
            MouseArea {
                anchors.fill: parent
                onClicked: {}
            }

            Text {
                id: filterTitle
                anchors.right: searchBar.left
                anchors.rightMargin: 10
                anchors.top: parent.top
                anchors.topMargin: 20
                anchors.left: parent.left
                anchors.leftMargin: 40
                text: qsTr("Filter list")
                font.weight: Font.DemiBold
                style: Text.Raised
                styleColor: "white"
            }

            Image {
                id: timezones
                anchors.top: searchBar.bottom
                anchors.topMargin: 10
                anchors.bottom: buttons.top
                anchors.left: parent.left
                anchors.bottomMargin: 10
                anchors.leftMargin: 40
                height: parent.height - buttons.height - 30
                width: parent.width - 80
                source: "image://theme/clock/bg_grooved_area"
                ListView {
                    id: tzlistmodel
                    anchors.fill: parent
                    clip: true
                    z: -1
                    model: timezonelist
                    highlight: highlighteroff
                    highlightMoveDuration: 1
                    delegate: Image {
                        id: timerect
                        property int gmt: gmtoffset
                        property string tzCity : city
                        property string tzTitle: title
                        source: "image://theme/clock/bg_list_item"
                        height: 30
                        width: parent.width
                        Text {
                            text: title
                            anchors.left: timerect.left
                            anchors.verticalCenter: parent.verticalCenter
                            color: theme_fontColorNormal
                            font.pointSize: theme_fontSizeMedium
                            font.bold: false
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                            wrapMode: Text.WordWrap
                        }
                        Text {
                            text: "(" + qsTr("GMT") + " " + ((gmtoffset < 0)?"":"+") + gmtoffset + ")"
                            anchors.right: timerect.right
                            anchors.verticalCenter: parent.verticalCenter
                            color: theme_fontColorNormal
                            font.pointSize: theme_fontSizeMedium
                            font.bold: false
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                            wrapMode: Text.WordWrap
                        }
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                tzlistmodel.currentIndex = index;
                                tzlistmodel.highlight = highlighter;
                                inputElement.text = title;
                            }
                        }
                    }
                }
            }

            Item {
                id: buttons
                height: 80
                width: parent.width
                anchors.bottom: parent.bottom
                Rectangle {
                    width: timezones.width
                    height: parent.height
                    anchors.centerIn: parent
                    color: "transparent"
                    Button {
                        id: saveButton
                        height: 50
                        width: 208
                        anchors.left: parent.left
                        anchors.verticalCenter: parent.verticalCenter
                        active: ((tzlistmodel.currentItem != undefined)&&(inputElement.displayText != ""))
                        bgSourceUp: "image://theme/btn_blue_up"
                        bgSourceDn: "image://theme/btn_blue_dn"
                        title: qsTr("Ok")
                        font.pixelSize: theme_fontSizeLargest
                        color: theme_buttonFontColor
                        onClicked: {
                            if((tzlistmodel.currentItem != undefined)&&(inputElement.displayText != ""))
                            {
                                container.triggered(tzlistmodel.currentItem.tzCity, tzlistmodel.currentItem.tzTitle, tzlistmodel.currentItem.gmt);
                                container.close();
                            }
                        }
                    }
                    Button {
                        id: cancelButton
                        height: 50
                        width: 208
                        anchors.right: parent.right
                        anchors.verticalCenter: parent.verticalCenter
                        bgSourceUp: "image://theme/btn_red_up"
                        bgSourceDn: "image://theme/btn_red_dn"
                        title: qsTr("Cancel")
                        font.pixelSize: theme_fontSizeLargest
                        color: theme_buttonFontColor
                        onClicked: {
                            container.close();
                        }
                    }
                }
            }

            Image {
                id: searchBar
                anchors.top: parent.top
                anchors.topMargin: 10
                anchors.right: parent.right
                anchors.rightMargin: 40
                width: parent.width - filterTitle.paintedWidth - 100
                source: "image://theme/clock/bg_searchbox"

                Image {
                    id: searchIcon
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    anchors.verticalCenter: parent.verticalCenter
                    source: "image://theme/clock/icn_search"
                }
                TextInput {
                    id: inputElement
                    anchors.left: searchIcon.right
                    anchors.top: parent.top
                    height: parent.height
                    width: parent.width - searchIcon.width - 30
                    anchors.margins: 10
                    font.pointSize: theme_fontSizeMedium
                    color: theme_fontColorNormal
                    focus: true
                    onTextChanged: {
                        timezonelist.filterOut(inputElement.displayText);
                        tzlistmodel.currentIndex = 0;
                        tzlistmodel.highlight = highlighter;
                    }
                    Keys.onReturnPressed: {
                        if((tzlistmodel.currentItem != undefined)&&(inputElement.displayText != ""))
                        {
                            container.triggered(tzlistmodel.currentItem.tzCity, tzlistmodel.currentItem.tzTitle, tzlistmodel.currentItem.gmt);
                            container.close();
                        }
                    }
                }
            }
        }
    }
}
