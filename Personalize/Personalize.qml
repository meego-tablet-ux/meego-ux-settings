/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at	
 * http://www.apache.org/licenses/LICENSE-2.0
 */

import Qt 4.7
import MeeGo.Labs.Components 0.1 as Labs
import MeeGo.Components 0.1 as MeeGo
import MeeGo.Media 0.1

Labs.ApplicationPage {
    id: container
    title: qsTr("Personalize")
    anchors.fill: parent

    Labs.ApplicationsModel {
        id: controlsModel
        type: "HomeConfig"
        directory: "/usr/share/meego-ux-appgrid/config"
    }

    Component {
        id: colorstripCreatorComponent
        ColorstripCreator {
            id: colorstripInstance
            anchors.fill: parent
        }
    }

    Item {
        id: personalizeContainer
        anchors.fill: container.content

        function close() {
            mainWindow.goHome();
            container.close();
        }

        ListView {
            id: listView
            anchors.fill: parent
            model: controlsModel.apps
            clip: true

            section.property: "msection"
            section.criteria: ViewSection.FullString
            section.delegate: Item {
                id: sinstance
                width: listView.width
                height: 40

                Text {
                    anchors.fill: parent
                    anchors.leftMargin: 10
                    elide: Text.ElideRight
                    color: "white"
                    font.pointSize: 10
                    font.bold: true
                    verticalAlignment: Text.AlignVCenter
                    text: {
                        if (section == 0)
                            sectionTitleWallpapers;
                        else if (section == 1)
                            sectionTitleClocks;
                        else if (section == 2)
                            sectionTitleWeather;
                        else if (section == 3)
                            sectionTitleCalendar;
                        else if (section == 4)
                            sectionTitleLocation;
                    }
                }
                Rectangle {
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 1
                    width: parent.width
                    height: 1
                    color: "black"
                }
            }
            header: Column {
                width: parent.width
                height: builtinWallpapers.height + customWallpapers.height + wallpaperTools.height
                MeeGo.PhotoPicker {
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
                    opacity: height > 0 ? 1.0 : 0.0
                    height: list.count > 0 ? 200 : 0
                }
                WallpaperTools {
                    id: wallpaperTools
                    width: parent.width
                    height: 100

                    onOpenColorstripCreator: {
                        function onCreatedColorbars(colorbarPath) {
                            var path = customWallpapers.model.copyImageToBackgrounds(colorbarPath);
                            customWallpapers.model.activeWallpaper = path;
                            mainWindow.goHome();
                            personalizeContainer.close();
                        }

                        var cb = colorstripCreatorComponent.createObject(personalizeContainer)
                        cb.scale = 1.0
                        cb.opacity = 1.0
                        cb.createdColorbars.connect(onCreatedColorbars)
                    }
                    onOpenGallery: {
                        photoPicker.show();
                    }
                }
            }

            delegate: Item {
                id: dinstance
                width:  listView.width

                function close() {
                    mainWindow.goHome();
                    personalizeContainer.close();
                }

                Component.onCompleted: {
                    var component = Qt.createComponent(modelData.value("Desktop Entry/X-MEEGO-HOME-CONTROL-SOURCE"));
                    var obj = component.createObject(dinstance);
                    height = obj.height
                    try
                    {
                        obj.desktop = modelData;
                    }
                    catch (err)
                    {
                        // Control file does not have a desktop property
                    }
                }
            }
        }
    }
}
