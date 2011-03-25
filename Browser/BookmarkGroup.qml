/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

import Qt 4.7
import MeeGo.Settings 0.1

GroupBox {
    id: bookmarkGroup
    title.text: qsTr("Bookmarks")
    width: parent.width
    loader.sourceComponent: bookmarkComponent

    Component
    {
        id: bookmarkComponent
        Column
        {
            id: bookmarkColumn
            //anchors.fill:parent
            spacing:-5
            height: bookmarkCheckbox.height - 5

            Connections {
                target: settings
                onShowBookmarkBarChanged: {
                    if(settings.showBookmarkBar) {
                        bookmarkGroup.infoText = qsTr("Always show")
                        bookmarkCheckbox.state = "checked";
                    }

                    else {
                        bookmarkGroup.infoText = qsTr("Never show")
                        bookmarkCheckbox.state = "unchecked"
                    }
                }
            }

            CheckboxButton {
                id: bookmarkCheckbox
                width: parent.width
                title.text: qsTr("Always show bookmarks bar")

                state: {
                   if(settings.showBookmarkBar) {
                       bookmarkGroup.infoText = qsTr("Always show")
                       return "checked";
                   }

                   else {
                       bookmarkGroup.infoText = qsTr("Never show")
                       return "unchecked"
                   }
                }
                onClicked: {
                    console.log("###" + state)
                    settings.showBookmarkBar = !settings.showBookmarkBar
//                    if(checked) bookmarkGroup.infoText = qsTr("Always show")
//                    else bookmarkGroup.infoText = qsTr("Never show")
                }
            }
        } // bookmarkColumn
    } // bookmarkComponent
}


