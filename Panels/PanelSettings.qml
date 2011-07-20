import Qt 4.7
import MeeGo.Labs.Components 0.1 as Labs
import MeeGo.Components 0.1
import MeeGo.Panels 0.1
import MeeGo.Settings 0.1

AppPage {
    pageTitle: qsTr("Panels")
    height: content.height

    PanelProxyModel{
        id: panelModel
        sortType: PanelProxyModel.SortTypeDefaultIndex
    }

    Theme {
        id: theme
    }

    Column {
        id: content
        width: parent.width

        /*Label {
        /// FIXME: This is in the page title.  we don't need it here too.
            text: qsTr("Panels")
            width: parent.width
            height: 60
        }*/

        Repeater {
            model: panelModel
            delegate:panelDelegate
            focus: true
        }


        Component {
            id: panelDelegate
            Item {
                id: imgPanel
                width: parent.width
                height: theme_listBackgroundPixelHeightOne

                ListSeparator {visible: index > 0}

                Text {
                    id: titleText
                    text: displayName
                    anchors.left: parent.left
                    anchors.leftMargin: 20
                    anchors.right: tbPanel.visible ? tbPanel.left : tbText.left
                    anchors.rightMargin: 12
                    color: theme_fontColorNormal
                    font.pixelSize: theme.fontPixelSizeNormal
                    verticalAlignment: Text.AlignVCenter
                    wrapMode: Text.NoWrap
                    elide: Text.ElideRight
                    height: parent.height
                }
                ToggleButton {
                    id: tbPanel
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    anchors.rightMargin: 20
                    visible: allowHide
                    on: isVisible
                    onToggled: {
                        panelObj.IsVisible = isOn;
                    }
                }
                Text {
                    id: tbText
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    anchors.rightMargin: 20
                    visible: !allowHide
                    color: theme_fontColorInactive
                    font.pixelSize: theme_fontPixelSizeNormal
                    text: qsTr("You can't turn off this panel")
                }
            }
        }
    }
}
