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

Item {
    id: container
    anchors.fill: parent

    property alias model: syncListView.model
    property ApplicationPage appPage
    property variant theLoginDialog: null

    Component {
          id: syncStorageHeading
          BorderImage {
              id: syncListData
              source: "image://theme/settings/btn_settingentry_dn"
              border.left: 5
              border.right: 5
              border.top: 5
              border.bottom: 5
              width: container.width
              height: storageHeadingLabel.height + 10

              Text {
                  id: storageHeadingLabel
                  anchors.verticalCenter: parent.verticalCenter
                  x: 5

                  color: theme_fontColorNormal
                  font.pixelSize: theme_fontPixelSizeLarger
                  font.bold: true

                  //: Arg 1 refers to the storage type (e.g. Contacts, Calendar, etc)
                  text: qsTr("Sync %1").arg(section)// storageName
              }
          }
      }

      ListView {
          id: syncListView
          anchors.margins: 10
          anchors.fill: parent
  
          clip: true
  
          delegate: SyncListDelegate {}
  
          section.property: "storage"
          section.criteria: ViewSection.FullString
          section.delegate: syncStorageHeading
      }
}
