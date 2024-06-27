// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause
import QtQuick
import QtWebView
import qmlmodels 1.0

ApplicationFlowForm {
    id: applicationFlow
    state: "Home"
    property string previousState: ""

    property int animationDuration: 400
    property string platform: Qt.platform.os
    property string mode: ""

    property string _nodeIP: ""
    property string _nodeName: ""

    //! [Theme button]
    function themeButton() {
        if (Colors.currentTheme == Colors.dark) {
            Colors.currentTheme = Colors.light
        } else {
            Colors.currentTheme = Colors.dark
        }
    }

    function toogleToFullScreen() {
        window.visibility = "FullScreen" // "Maximized"
    }

    function toogleToWindowScreen() {
        window.visibility = "Windowed"
    }

    function backButton() {
        stack.pop()
        toogleToWindowScreen()
        applicationFlow.state = applicationFlow.previousState
    }

    //! [On clicked]
    home.localButton.onClicked: {
        applicationFlow.state = "Local"
        stack.push(localPage)
    }

    home.remoteButton.onClicked: {
        applicationFlow.state = "Remote"
        stack.push(remotePage)
    }

    Component {
        id: localPage
        Local {
            visible: true
            state: applicationFlow.mode
        }
    }

    Component {
        id: remotePage
        Remote {
            visible: true
            state: applicationFlow.mode
//          nodesList: nodeItemListModel
//          nodesList: nodeList
            Component.onCompleted: {
                sendNodeClicked.connect(onNodeClicked)
            }

        }
    }

    Component {
        id: remoteNodePage
        RemoteNode {
            id: remoteNodeComp
            visible: true
            state: applicationFlow.mode
            Component.onCompleted: {
                remoteNodeComp.nodeUrl = _nodeIP
                remoteNodeComp.nodeName = _nodeName
            }
        }

    }

    function onNodeClicked(nodeAddress, nodeName) {
        applicationFlow.state = "RemoteIP"
        _nodeIP = nodeAddress
        _nodeName = nodeName
        stack.push(remoteNodePage)
    }

/*
    function onReturnToStart() {
        stack.pop(stack.get(0))
        applicationFlow.state = "Home"
    }
*/

    //! [States]
    states: [
        State {
            name: "Home"
            PropertyChanges {
                target: toolbar
                anchors.topMargin: parent.height / 80
                backButton.opacity: 0
                backButton.enabled: false
                themeButton.opacity: 100
                themeButton.enabled: true
                logo.sourceSize.width: 50
                logo.sourceSize.height: 40
                logo.visible: true
                visible: true
                height: 35
            }
            PropertyChanges {
                target: applicationFlow
                previousState: ""

            }

            PropertyChanges {
                target: stack
                anchors.top: toolbar.bottom
                anchors.topMargin: parent.height / 20
            }
            PropertyChanges {
                target: window
            }
        },
        State {
            name: "Local"
            PropertyChanges {
                target: toolbar
                anchors.topMargin: 0
                backButton.opacity: 100
                backButton.enabled: true
                themeButton.opacity: 100
                themeButton.enabled: true
//              logo.sourceSize.width: 70
//              logo.sourceSize.height: 50
                logo.visible: true
                visible: true
            }

            PropertyChanges {
                target: applicationFlow
                previousState: "Home"

            }

            PropertyChanges {
                target: stack
                anchors.top: toolbar.bottom
                anchors.topMargin: 0
            }
        },
        State {
            name: "Remote"
            PropertyChanges {
                target: toolbar
                anchors.topMargin: 0
                backButton.opacity: 100
                backButton.enabled: true
                themeButton.opacity: 100
                themeButton.enabled: true
                logo.visible: true
                visible: true
            }

            PropertyChanges {
                target: applicationFlow
                previousState: "Home"

            }

            PropertyChanges {
                target: stack
                anchors.top: toolbar.bottom
                anchors.topMargin: 0
            }
        },
        State {
            name: "RemotePage"

            PropertyChanges {
                target: applicationFlow
                previousState: "Remote"
            }
        }
    ]
}
