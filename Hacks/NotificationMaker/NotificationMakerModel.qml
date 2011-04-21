/*
* Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

import Qt 4.7
import MeeGo.Settings 0.1

ListModel {
    ListElement {
        name: "Missed call"
        eventType: "x-nokia.call"
    }
    ListElement {
        name: "Missed Video call / Pending chat message"
        eventType: "im"
    }
    ListElement {
        name: "App Update"
        eventType: "app.update"
    }
    ListElement {
        name: "New IM"
        eventType: "im.received"
    }
    ListElement {
        name: "Bluetooth Deviced Disconnected"
        eventType: "bluetooth.devicedisconnected"
    }
    ListElement {
        name: "New Email"
        eventType: "email.arrived"
    }
    ListElement {
        name: "Social Web Friend Request"
        eventType: "social.friendrequest"
    }
    ListElement {
        name: "Download Completed"
        eventType: "transfer.complete"
    }

    ListElement {
        name: "Device added event"
        eventType: "device.added"
    }
    ListElement {
        name: "Device removed event"
        eventType: "device.removed"
    }
    ListElement {
        name: "Email bounced"
        eventType: "email.bounced"
    }
    ListElement {
        name: "Network Error"
        eventType: "network.error"
    }

}
