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

ListView {
    id: delegateParent

    property int selectedIndex

    signal triggered ( int index )

    width: parent.width
    height: parent.height

    property int    maxTextWidth: 0
    property int    maxIconWidth: 0
    property bool   firstSection: true
    property string sectionValue: ""

    section.property: "section"
    section.delegate: WindowSideBarSectionDelegate { }
    section.criteria: ViewSection.FullString

    delegate:  WindowSideBarDelegate {
        id: delegate
        onTriggered: { delegateParent.triggered(index) }
    }
}
