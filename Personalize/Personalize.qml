/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at	
 * http://www.apache.org/licenses/LICENSE-2.0
 */

import Qt 4.7
import MeeGo.Labs.Components 0.1 as Labs
import MeeGo.Components 0.1
import MeeGo.Media 0.1
import MeeGo.Panels 0.1

AppPage {
    id: container
    pageTitle: qsTr("Personalize")
    height: contentArea.height + 10

    PanelProxyModel{
        id: panelModel
        sortType: PanelProxyModel.SortTypeDefaultIndex
    }

    Column {
        id: contentArea
        width: parent.width

         PhotoPicker {
            id: photoPicker
            parent: personalizeContainer
            property string selectedPhoto

            albumSelectionMode: false
            onPhotoSelected: {
                selectedPhoto = uri.split("file://")[1];
            }
            onAccepted: {
                if (selectedPhoto)
                {
                    var path = customWallpapers.model.copyImageToBackgrounds(selectedPhoto);
                    customWallpapers.model.activeWallpaper = path;
                    mainWindow.goHome();
                    personalizeContainer.close();
                }
            }
        }

       BuiltInWallpapers {
            id: builtinWallpapers
            width: parent.width
            height: 200
        }

        CustomWallpapers {
            id: customWallpapers
            width: parent.width
            visible: height > 0
            height: list.count > 0 ? 200 : 0
        }

        WallpaperTools {
            id: wallpaperTools

            onOpenGallery: {
                photoPicker.show();
            }
        }

        Label {
            text: qsTr("Panels")
            width: parent.width
            height: 60
        }

        Repeater {
            model: panelModel
            delegate:panelDelegate
            focus: true
        }

        Component {
            id: panelDelegate
            Image {
                    id: imgPanel
                    source: "image://themedimage/images/settings/btn_settingentry_up"
                    width: parent.width


                Text {
                    id: titleText
                    text: displayName
                    anchors.left: parent.left
                    anchors.leftMargin: 20
                    anchors.right: tbPanel.visible ? tbPanel.left : tbText.left
                    anchors.rightMargin: 12
                    color: theme_fontColorNormal
                    font.pixelSize: theme_fontPixelSizeLarge
                    verticalAlignment: Text.AlignVCenter
                    wrapMode: Text.NoWrap
                    elide: Text.ElideRight
                    height: parent.height
                }
                ToggleButton {
                    id: tbPanel
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    anchors.rightMargin: 20
                    visible: allowHide
                    on: isVisible
                    onToggled: {
                        panelObj.IsVisible = isOn;
                    }
                }
                Text {
                    id: tbText
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    visible: !allowHide
                    color: theme_fontColorInactive
                    font.pixelSize: theme_fontPixelSizeNormal
                    text: qsTr("You can't turn off this panel")
                }
            }
        }
    }
}
