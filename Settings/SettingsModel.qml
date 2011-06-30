/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

import Qt 4.7
import MeeGo.Labs.Components 0.1 as Labs

ListModel {

	property QtObject desktopSettingsModel: Labs.ApplicationsModel {
			id: desktopSettingsModel
			type: "ControlPanelApplet"
			directories: ["/usr/share/meego-ux-settings/apps/"]
		}


	id: settingsModel

	property Component declarativeComponent
	property variant settingsApps: []
	property variant settingsAppNames: []
	property variant settingsAppPaths: []
	property variant settingsAppComponents: []
	property variant settingsTranslationPaths: []
	property int firstSectionNextIndex: 0

	function filter(filterValue) {
		settingsApps = []
		settingsAppPaths = []
		settingsModel.clear();
		filterValue = filterValue.toLowerCase();
		console.log("filter: " + filterValue)

		for (var i=0; i < desktopSettingsModel.apps.length; i++) {

			var app = desktopSettingsModel.apps[i];
			console.log("index of filterValue in title: " + app.title.indexOf(filterValue))

			if(app.title.toLowerCase().indexOf(filterValue) != -1 || app.title.toUpperCase().indexOf(filterValue) != -1) {

				appendApp(app)
			}
		}
	}


	Component.onCompleted: {
		console.log("number of settings: " + desktopSettingsModel.apps.length)

		var sortedList=[];

		///presort the array:
		for (var i=0; i < desktopSettingsModel.apps.length; i++) {
			var app = desktopSettingsModel.apps[i];

			var section = app.value("MTS/Section")

			if (section == "Settings") {
				sortedList.splice(settingsModel.firstSectionNextIndex++,0,app)
			}
			else {
				var len = sortedList.push(app)
			}
		}

		for(var i=0; i < sortedList.length; i++) {
			appendApp(sortedList[i])
		}
	}

	function reloadModel() {

		settingsModel.clear();

		var sortedList=[];

		///presort the array:
		for (var i=0; i < desktopSettingsModel.apps.length; i++) {
			var app = desktopSettingsModel.apps[i];

			var section = app.value("MTS/Section")

			if (section == "Settings") {
				sortedList.splice(settingsModel.firstSectionNextIndex++,0,app)
			}
			else {
				var len = sortedList.push(app)
			}
		}

		for(var i=0; i < sortedList.length; i++) {
			appendApp(sortedList[i])
		}
	}

	function appendApp(app)
	{
		var title = app.title
		var name = app.name
		if(name == "") {
			console.log("************************************************************")
			console.log("* WARNING: " + title + " is missing 'Name' section         *")
			console.log("* Any remote calls to the page may fail.                   *")
			console.log("* please add 'Name'' to the settings .desktop              *")
			console.log("************************************************************")
		}

		var icon = app.icon;
		var path = app.value("MTS/Part");
		var translationFile = app.value("MTS/Translation")
		var section = app.value("MTS/Section")

		var sectionMap = [];
		sectionMap["Settings"] = qsTr("Settings")
		sectionMap["Applications"] = qsTr("Application","should actually by 'Applications'")
		sectionMap[""] = qsTr("Application","should actually by 'Applications'")

		if(path=="") {
			path = app.value("DCP/Part")
		}
		settingsApps = settingsApps.concat(title);
		settingsAppPaths = settingsAppPaths.concat(path);
		//settingsAppComponents = settingsAppComponents.concat(Qt.createComponent(path))
		settingsTranslationPaths = settingsTranslationPaths.concat(translationFile)
		settingsAppNames = settingsAppNames.concat(name)

		settingsModel.append({ "id": name, "name": title,
							 "path": path, "icon": icon, "translation": translationFile,
							 "section": sectionMap[section] })
	}

}
