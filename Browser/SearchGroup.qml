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
    id: searchGroupBox
    title.text: qsTr("Search Engine")
    width: parent.width
    loader.sourceComponent: listviewComponent
    state:"elapsed"
    infoText: qsTr("Google")

    Component {
        id: listviewComponent
        ListView {
            id: searchEngineListView
            height: (60 + spacing)*count
            contentHeight: 60*count
            interactive: false
            spacing:0
            clip:true
            model: settings.searchEngineList

/*            Connections {
                target: settings
                onDefaultSearchEngineChanged: {
                    searchEngineListView.currentItem.checked = false;
                    searchEngineListView.currentIndex = 0;
                    searchEngineListView.currentItem.checked = true;
                }
            }
*/
            delegate: CheckboxButton {
                width: searchEngineListView.width
                title.text: modelData
                state: {
                    if(modelData == settings.defaultSearchEngine) {
                        searchEngineListView.currentIndex = index;
                        searchGroupBox.infoText = modelData
                        return "checked";
                    } else { return "unchecked";}
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        //if(!checked) {
                            searchEngineListView.currentItem.checked = false;
                            searchEngineListView.currentIndex = index;
                            searchGroupBox.infoText = modelData
                            settings.defaultSearchEngine = modelData
                            checked = true;
                        //}
                    }
                }
            }
        }
    }
}

