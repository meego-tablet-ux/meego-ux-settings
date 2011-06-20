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

Column {
    id: delegateParent

    property alias model: repeater.model
    property alias delegate: repeater.delegate
    property int selectedIndex

    signal triggered ( int index )

    width: delegate.width

    property int    maxTextWidth: 0
    property int    maxIconWidth: 0
    property string sectionValue: ""

    onSelectedIndexChanged: {
        console.log("selectedIndex changed to:", selectedIndex)
        triggered(selectedIndex)
    }

    Repeater {
        id: repeater

        delegate:  WindowSideBarDelegate {
            id: delegate
            onTriggered: { delegateParent.triggered(index) }
        }
    }
}
