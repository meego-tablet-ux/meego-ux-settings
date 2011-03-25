/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

import Qt 4.7
import MeeGo.Labs.Components 0.1

Item {
    id: container
    anchors.fill: parent
    focus: true

    TopItem {
        id: top
    }

    property string username
    property string password
    property string serviceName
    property Item loginOwner

    Component {
        id: loginDataEntry
	
        Item {
            anchors.fill: parent

            Column {
                anchors.centerIn: parent
		spacing: 10

                TextEntry {
                    id: usernameField

                    width: 400

                    //: Username example text.  Note: do not translate "example.com"!
                    property string example: qsTr("(ex: foo@example.com)")

                    //: Sync account username (e.g. foo.bar@yahoo.com) login field label, where arg 1 is an example, which may or not be visible.
                    defaultText: qsTr("Username %1").arg(example)
                    text: username
                    inputMethodHints: Qt.ImhEmailCharactersOnly | Qt.ImhNoAutoUppercase

                    // Set up an e-mail address validator
                    //
                    // THIS REGULAR EXPRESSION DOES NOT COVER SOME RARE E-MAIL ADDRESS
                    // CORNER CASES.
                    //
                    textInput.validator: RegExpValidator {
//                        regExp: /^([a-zA-Z0-9_\.\-\+])+\@(([a-zA-Z0-9\-])+\.)+([a-zA-Z0-9]{2,4})+$/
                        regExp: /^[a-zA-Z0-9._-]+@[a-zA-Z0-9.-]+.[a-zA-Z]{2,4}$/
                    }

                    Keys.onTabPressed: {
                        passwordField.textInput.focus = true;
                    }
                    Keys.onReturnPressed: {
                        dialogClicked(1);  // Simulate pressing the sign-in button.
                    }
                }

                TextEntry {
                    id: passwordField

                    width: usernameField.width

                    //: Sync account password login field label
                    defaultText: qsTr("Password")
                    text: password
                    inputMethodHints: Qt.ImhNoAutoUppercase

                    textInput.echoMode: TextInput.Password

                    Keys.onTabPressed: {
                        usernameField.textInput.focus = true;
                    }
                    Keys.onReturnPressed: {
                        dialogClicked(1);  // Simulate pressing the sign-in button.
                    }
                }
	    }

            Connections {
                target: loginDialog
                onDialogClicked: {
                    if (button == 1
                        && usernameField.text != "") {
                        if (usernameField.textInput.acceptableInput) {
                            // Sign in

                            // Done entering login information.  Now execute the desired action.
                            loginOwner.executeOnSignin(usernameField.text, passwordField.text);

                            // Close the Dialog.
                            container.destroy();
                        } else {
                            // Display the sample username again.
                            usernameField.text = ""
                        }
                    } else if (button == 2) {
                        // Cancel

                        // Close the Dialog.
                        //dialogLoader.sourceComponent = undefined;
                        container.destroy();
                    }
                }
            }
        }
    }

    ModalDialog {
        id: loginDialog
        parent: top.topItem
        anchors.fill: parent

        //: "Sign in" button text displayed in sync account login dialog.
        leftButtonText: qsTr("Sign in")
        //: "Cancel" button text displayed in sync account login dialog.
        rightButtonText: qsTr("Cancel")
        //: The argument is the name of the remote sync service (e.g. Google, Yahoo!, etc).
        dialogTitle: qsTr("Sign in to your %1 account").arg(serviceName)
        dialogHeight: 250
        contentLoader.sourceComponent: loginDataEntry
        contentLoader.focus: true
	
    }

}

