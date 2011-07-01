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
import MeeGo.Settings 0.1

Item {
    id: languageItem
    width: parent.width
    height: dropDown.height+50

    Labs.LocaleHelper { id: localehelper }

    Labs.LocaleListModel { id: localeModel }

    DropDown {
        id: dropDown
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 10
        width: parent.width / 2
        model: localeModel.displayLocales()
        payload: localeModel.locales()
        selectedIndex: localeModel.indexOf(localehelper.locale)

        onTriggered: {
            localehelper.locale = payload[index]
        }
    }
}

