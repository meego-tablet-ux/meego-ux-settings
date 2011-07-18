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
import MeeGo.Settings 0.1

AppPage {
    id: container
    pageTitle: qsTr("Personalize")
    height: contentArea.height + 10

    PanelProxyModel{
        id: panelModel
        sortType: PanelProxyModel.SortTypeDefaultIndex
    }

    PhotoPicker {
        id: photoPicker
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
    Column {
        id: contentArea
        width: parent.width


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
    }
}
