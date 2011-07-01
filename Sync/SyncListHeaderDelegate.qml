import QtQuick 1.0
import MeeGo.Components 0.1

BorderImage {
  id: syncListData
  source: "image://theme/settings/btn_settingentry_dn"
  border.left: 5
  border.right: 5
  border.top: 5
  border.bottom: 5
  width: parent.width
  height: storageHeadingLabel.height + 10

  Theme {
      id: theme
  }

  Text {
    id: storageHeadingLabel
    anchors.verticalCenter: parent.verticalCenter
    x: 5

    color: theme.fontColorNormal
    font.pixelSize: theme.fontPixelSizeLarge
    font.bold: true

    //: Arg 1 refers to the storage type (e.g. Contacts, Calendar, etc)
    text: qsTr("Sync %1").arg(section)
  }
}
