/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

import Qt 4.7
import MeeGo.Labs.Components 0.1
import MeeGo.Settings 0.1

Item {
    id: container

    property alias label: itemCrumb.label
    property variant topics
    property variant model
	property variant apps: []
	property int mode: 0
    function topicTriggered(index) {
		console.log("attempting to load: " + apps[index])
		contentLoader.source = apps[index];
		contentLoader.item.parent = parent;

    }

	function declarativeTopicTriggered(src) {
		contentLoader.sourceComponent = declarativeComponent;
		console.log("setting document source to " + src)
		contentLoader.item.source = src;
		contentLoader.item.parent = parent;
	}

    property alias crumb: itemCrumb
    Crumb {
        id: itemCrumb
    }

	Loader {
		id: contentLoader
	}

	Component {
		id: declarativeComponent
		DeclarativeComponent { }
	}

}
