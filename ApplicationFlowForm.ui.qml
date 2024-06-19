/*
This is a UI file (.ui.qml) that is intended to be edited in Qt Design Studio only.
It is supposed to be strictly declarative and only uses a subset of QML. If you edit
this file manually, you might introduce QML code that is not supported by Qt Design Studio.
Check out https://doc.qt.io/qtcreator/creator-quick-ui-forms.html for details on .ui.qml files.
*/

import QtQuick 6.2
import QtQuick.Controls 6.2
import unicon_launcher
import QtWebView

Rectangle {
    id: root
    property alias toolbar: toolbar
    property alias stack: stack
    property alias home: home

    width: Constants.width
    height: Constants.height

    color: Colors.currentTheme.background

    CustomToolBar {
        id: toolbar
        anchors.topMargin: parent.height / 80
        width: parent.width
        height: 35
        anchors.top: parent.top
    }
/*
    Text {
        id: pageText
        text: "Selection"
        font.pixelSize: 24
        font.family: "Titillium Web"
        font.weight: 700
        color: Colors.currentTheme.textColor
        anchors.top: toolbar.bottom
        anchors.topMargin: parent.height / 20
        leftPadding: 20
    }
*/


    //! [Stack view]
    StackView {
        id: stack
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.topMargin: parent.height / 20
        visible: true
        initialItem: Home {
            id: home
            visible: true
            state: applicationFlow.mode
        }

        pushEnter: Transition {
            PropertyAnimation {
                property: "x"
                from: stack.width
                to: 0
                duration: 400
            }
        }
        //! [Stack view]
        pushExit: Transition {
            PropertyAnimation {
                property: "x"
                from: 0
                to: -stack.width
                duration: 400
            }
        }
        popEnter: Transition {
            PropertyAnimation {
                property: "x"
                from: -stack.width
                to: 0
                duration: 400
            }
        }
        popExit: Transition {
            PropertyAnimation {
                property: "x"
                from: 0
                to: stack.width
                duration: 400
            }
        }
    }
}
