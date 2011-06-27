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
    height: sectionDelegate.height + itemDelegate.height

    onExpandChanged: {
        console.log("expand changed to: " + expand)
    }

    Item {
        id: sectionDelegate
        anchors.left:  parent.left
        anchors.right:  parent.right
        anchors.top:  parent.top
        height: visible ? 50 : 0 //TODO: may be different

        visible: true

        ThemeImage {
            id: sectionImage

            anchors {
                left: parent.left
                right:parent.right
            }
            height: sectionDelegate.height

            source: "image://themedimage/widgets/common/header/header-inverted-small-top"

            LayoutTextItem {
                id: sectionText

                anchors.fill:  parent
                anchors.margins:  10

                verticalAlignment: Text.AlignVCenter
                horizontalAlignment:Text.AlignLeft
                elide: Text.ElideRight

                text: model.section ? model.section : ""
                font.pixelSize: theme.fontPixelSizeLarge

                Component.onCompleted: {
                    if (text && (model.section != delegateParent.sectionValue)) {
                        delegateParent.sectionValue = text
                        if (0 == index) {
                            sectionImage.source = "image://themedimage/widgets/common/header/header-inverted-small-top"
                        }
                        else {
                            sectionImage.source = "image://themedimage/widgets/common/header/header-inverted-small"
                        }
                        sectionDelegate.visible = true
                    }
                    else {
                        sectionDelegate.visible = false
                    }
                }
            }
        }
    } //end sectionDelegate

    Item {
        id: itemDelegate

        property bool isSelected: (delegateParent.selectedIndex == index)
        property bool isActive: false

        anchors {
            left: parent.left
            right: parent.right
            top: sectionDelegate.bottom
        }
        height: 50  //TODO: may be different

        ThemeImage {
            id: itemBackground
            anchors.fill: parent

            source: itemDelegate.isActive ? "image://themedimage/widgets/common/action-item/action-item-background-active"
                                          : "image://themedimage/widgets/common/action-item/action-item-background-selected"
            visible: itemDelegate.isSelected || itemDelegate.isActive
        }

        ThemeImage {
            id: itemDivider

            anchors {
                top:  parent.top
                left:  parent.left
                right:  parent.right
                leftMargin: 10
                rightMargin: 10
            }

            source: "image://themedimage/widgets/common/dividers/divider-horizontal-double"
            visible: ! sectionDelegate.visible
        }

        Item {
            id: bookBarIconArea
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.margins:  delegateParent.maxIconWidth ? 10 : 0

            height: parent.height
            width:  delegateParent.maxIconWidth

            ThemeImage {
                id: bookBarIcon
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter

                //visible:  index < bookMenuIconModel.size()
                source: model.icon ? model.icon : ""
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

            text: model.name ? model.name : modelData
            color: itemDelegate.isActive ? "white" : (itemDelegate.isSelected ? "#2fa7d4" : "#3d3d3d")

            Component.onCompleted: {
                if (bookBarText.width > delegateParent.maxTextWidth) {
                    delegateParent.maxTextWidth = bookBarText.width
                }
            }
        }

        GestureArea {
            anchors.fill: itemDelegate
            acceptUnhandledEvents: false

            Tap {
                onFinished: {
                    delegateParent.selectedIndex = index
                    itemDelegate.isActive = false
                    triggered( index )
                }

                onStarted:  { itemDelegate.isActive = true }
                onCanceled: { itemDelegate.isActive = false }
            }

            TapAndHold {
                onFinished: {
                    delegateParent.selectedIndex = index
                    itemDelegate.isActive = false
                    triggered( index )
                }

                onStarted:  { itemDelegate.isActive = true }
                onCanceled: { itemDelegate.isActive = false }
            }

            Swipe {
                onStarted:  { itemDelegate.isActive = false }
                onCanceled: { itemDelegate.isActive = false }
                onFinished: { itemDelegate.isActive = false }
            }
            Pan {
                onStarted:  { itemDelegate.isActive = false }
                onCanceled: { itemDelegate.isActive = false }
                onFinished: { itemDelegate.isActive = false }
            }
        }


    } //end itemDelegate
}
