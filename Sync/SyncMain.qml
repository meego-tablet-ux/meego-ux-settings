/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

import QtQuick 1.0
import MeeGo.Components 0.1
import MeeGo.Sync 0.1

AppPage {
    id: syncPage
    //: The title of the Sync UI displayed to the user.
    pageTitle: qsTr("Sync Settings")
    height: window.height - window.barsHeight - window.topDecorationHeight

    SyncList {
      id: syncAllProfilesList
      anchors.fill: parent
    }
}
