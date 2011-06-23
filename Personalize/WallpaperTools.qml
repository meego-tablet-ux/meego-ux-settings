/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at	
 * http://www.apache.org/licenses/LICENSE-2.0
 */

import Qt 4.7
import MeeGo.Components 0.1

Button {
    id: container

    signal openColorstripCreator()
    signal openGallery()

    width: parent.width/2 - 20
    elideText: true
    //height: parent.height * 0.75
    text: qsTr("Pick a photo")
    onClicked: {
        container.openGallery()
    }
}
