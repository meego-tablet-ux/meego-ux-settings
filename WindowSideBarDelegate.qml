/*
* Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * LGPL, version 2.1.  The full text of the LGPL Licence is at
 * http://www.gnu.org/licenses/lgpl.html
 */

import Qt 4.7
import MeeGo.Ux.Kernel 0.1
import MeeGo.Ux.Gestures 0.1
import MeeGo.Ux.Components.Common 0.1

// Assumes that the parent for this delegate is "delegateParent" and has properties
// property int maxTextWidth
// property int maxIconWidth
// property string section

Item {
    id: root

    property bool expand: true
    signal triggered ( int index )

    //width:  bookBarIconArea.width + bookBarText.width + 10*3
    width:  parent.width
    height: itemDelegate.height

    onExpandChanged: {
        console.log("expand changed to: " + expand)
    }

    Item {
        id: itemDelegate

        property bool isSelected: (delegateParent.selectedIndex == index)

        width: parent.width

        height: 50  //TODO: may be different

        ThemeImage {
            anchors.fill: parent

            source: "image://themedimage/widgets/common/action-item/action-item-background-active"
            visible: itemDelegate.isSelected
            opacity:  0.1
        }

        ThemeImage {
            id: bookBarBackground

            anchors {
                top:  parent.top
                left:  parent.left
                right:  parent.right
                leftMargin: 10
                rightMargin: 10
            }

            source: "image://themedimage/widgets/common/dividers/divider-horizontal-double"

        }

        Item {
            id: bookBarIconArea
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.margins:  10

            height: parent.height
            width:  delegateParent.maxIconWidth

            ThemeImage {
                id: bookBarIcon
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter

                //visible:  index < bookMenuIconModel.size()
                source: model.icon
                height: bookBarIconArea.height - 10
                width: sourceSize.width * height / sourceSize.height

                Component.onCompleted: {
                    if (bookBarIcon.width > delegateParent.maxIconWidth) {
                        delegateParent.maxIconWidth = bookBarIcon.width
                    }
                }
            }
        }

        LayoutTextItem {
            id: bookBarText

            anchors.verticalCenter: parent.verticalCenter
            anchors.left: bookBarIconArea.right
            anchors.leftMargin: 10
            font.pixelSize: theme.fontPixelSizeNormal

            maxWidth: root.width - bookBarIconArea.width - 10*3

            verticalAlignment: Text.AlignVCenter
            horizontalAlignment:Text.AlignLeft
            elide: Text.ElideRight

            text: model.name
            color: itemDelegate.isSelected ? "#2fa7d4" : "#3d3d3d"

            Component.onCompleted: {
                if (bookBarText.width > delegateParent.maxTextWidth) {
                    delegateParent.maxTextWidth = bookBarText.width
                }
            }
        }

        MouseArea {
            anchors.fill: itemDelegate
            onClicked: {
                console.log("Clicked item", index)
                delegateParent.selectedIndex = index
                triggered(index)
            }
        }

    } //end itemDelegate
}
