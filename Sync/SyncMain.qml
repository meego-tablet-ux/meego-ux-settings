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
    function calculateHeight()
    {
      var ret = ((window.inLandscape || window.inInvertedLandscape)
              ? (window.height - window.barsHeight - window.topDecorationHeight)
              : (window.width  - window.barsHeight - window.topDecorationHeight));

      return ret;
    }

    id: syncPage
    //: The title of the Sync UI displayed to the user.
    pageTitle: qsTr("Sync Settings")
    height: calculateHeight()

    Connections {
      target: window
      onOrientationChanged: {
        syncPage.height = calculateHeight();
      }
    }

    SyncList {
      id: syncAllProfilesList
    }
}
