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
    anchors.fill: parent

    // width: Constants.width
    // height: Screen.desktopAvailableHeight*0.8// < Constants.height ? Screen.desktopAvailableHeight*0.9 : Constants.height

    color: Colors.currentTheme.background

    CustomToolBar {
        id: toolbar
        anchors.topMargin: 0 //parent.height / 80
        anchors.top: parent.top
        width: parent.width
        visible: false
        height: 35
    }

    //! [Stack view]
    StackView {
        id: stack
        anchors.top: toolbar.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.topMargin: 25//parent.height / 20
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
