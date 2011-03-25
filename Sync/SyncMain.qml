/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

import Qt 4.7
import MeeGo.Labs.Components 0.1
import MeeGo.Sync 0.1

ApplicationPage {
    id: appContainer
    //: The title of the Sync UI displayed to the user.
    title: qsTr("Sync Settings")
    anchors.fill: parent

    AllVisibleSyncProfiles {
        syncParentPage: appContainer
        parent: appContainer.content
        anchors.fill: parent
    }
}
