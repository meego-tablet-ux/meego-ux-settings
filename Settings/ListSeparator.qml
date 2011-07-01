/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */

import Qt 4.7
Item {
    height: 2
    anchors.left: parent.left
    anchors.leftMargin: 10
    anchors.right: parent.right
    anchors.rightMargin: 10
    Rectangle {
        id: spaceLineDark
        color: theme_separatorDarkColor
        opacity: theme_separatorDarkAlpha
        height: 1
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
    }
    Rectangle {
        id: spaceLineLight
        color: theme_separatorLightColor
        opacity: theme_separatorLightAlpha
        height: 1
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
    }
}
