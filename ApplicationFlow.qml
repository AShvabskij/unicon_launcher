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
    property string _nodeRoute: ""
    property string _nodeName: ""

    signal sendRefreshClicked()
    signal sendDemoClicked()

    //! [Theme button]
    function themeButton() {
        if (Colors.currentTheme == Colors.dark) {
            Colors.currentTheme = Colors.light
            Constants.isDarkTheme = false
        } else {
            Colors.currentTheme = Colors.dark
            Constants.isDarkTheme = true
        }
    }

    function settingsButton() {
        applicationFlow.previousState = applicationFlow.state
        applicationFlow.state = "Settings"
        stack.push(settingsPage)
    }

    function aboutButton() {
        applicationFlow.previousState = applicationFlow.state
        applicationFlow.state = "About"
        stack.push(aboutPage)
    }

    function refreshButton() {
        if (applicationFlow.state == "Local") {
            sendRefreshClicked()
        } else if (applicationFlow.state == "Remote") {
            sendRefreshClicked()
        } else if (applicationFlow.state == "RemoteIP") {
            sendRefreshClicked()
        }
    }

    function demoButton() {
        if (applicationFlow.state == "Local") {
            sendDemoClicked()
        }
    }

    function toggleToFullScreen() {
        window.visibility = "Maximized" //"FullScreen"
    }

    function toggleToWindowScreen() {
        window.visibility = "Windowed"
    }

    function onWebOpened() {
        toolbar.theme = Colors.light;
        if (applicationFlow.state === "Local") {
            toolbar.demoButton.visible = true;
        }
    }


    function backButton() {
        stack.pop()
        toggleToWindowScreen()

        if (applicationFlow.state === "Settings" && applicationFlow.previousState === "Remote") {
            sendRefreshClicked()
        }

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

    NodeListModel {
        id: nodeListModel
    }

    Component {
        id: localPage
        Local {
            visible: true
            state: applicationFlow.mode
            Component.onCompleted: {
                sendRefreshClicked.connect(onRefreshPage)
                sendDemoClicked.connect(onDemoPage)
            }
        }
    }

    Component {
        id: remotePage
        Remote {
            visible: true
            state: applicationFlow.mode
            nodesModel: nodeListModel
            Component.onCompleted: {
                sendNodeClicked.connect(onNodeClicked)
                sendRefreshClicked.connect(onRefreshPage)
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
                remoteNodeComp.nodeIP = _nodeIP
                remoteNodeComp.nodeRoute = _nodeRoute
                remoteNodeComp.nodeName = _nodeName
                sendRefreshClicked.connect(onRefreshPage)
            }
        }
    }

    Component {
        id: settingsPage
        Settings {
            id: settingsComp
            visible: true
            state: applicationFlow.mode
        }
    }

    Component {
        id: aboutPage
        About {
            id: aboutComp
            visible: true
            state: applicationFlow.mode
        }
    }

    function onNodeClicked(nodeAddress, nodeRoute, nodeName) {
        applicationFlow.state = "RemoteIP"
        _nodeIP = nodeAddress
        _nodeRoute = nodeRoute
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
                themeButton.opacity: 0
                // themeButton.enabled: true
                settingsButton.visible: false
                aboutButton.visible: true
                refreshButton.visible: false
                demoButton.visible: false
                backButton.visible: false

                height: 0
                visible: true
            }
            PropertyChanges {
                target: applicationFlow
                previousState: ""

            }

            PropertyChanges {
                target: stack
//              anchors.top: toolbar.bottom
//              anchors.topMargin: 0 //parent.height / 20
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
                backButton.visible: true
                themeButton.opacity: 0
                themeButton.enabled: true
                refreshButton.visible: true
                demoButton.visible: false
                settingsButton.visible: false
                theme: Colors.currentTheme
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
                backButton.visible: true
                themeButton.opacity: 0
                themeButton.enabled: true
                refreshButton.visible: true
                demoButton.visible: false
                settingsButton.visible: true
                aboutButton.visible: true
                theme: Colors.currentTheme
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
            name: "RemoteIP"

            PropertyChanges {
                target: toolbar
                anchors.topMargin: 0
                backButton.visible: true
                themeButton.opacity: 0
                themeButton.enabled: true
                refreshButton.visible: true
                demoButton.visible: false
                aboutButton.visible: false
                theme: Colors.currentTheme
                visible: true
            }

            PropertyChanges {
                target: applicationFlow
                previousState: "Remote"
            }
        },
        State {
            name: "Settings"
            PropertyChanges {
                target: toolbar
//              anchors.topMargin:  parent.height / 80
                themeButton.opacity: 0
                settingsButton.visible: false
                aboutButton.visible: false
                refreshButton.visible: false
                demoButton.visible: false
                backButton.visible: true
                theme: Colors.currentTheme
                visible: true
            }

            PropertyChanges {
                target: applicationFlow
                previousState: "Remote"
            }
        },
        State {
            name: "About"
            PropertyChanges {
                target: toolbar
//              anchors.topMargin:  parent.height / 80
                themeButton.opacity: 0
                settingsButton.visible: false
                aboutButton.visible: false
                refreshButton.visible: false
                demoButton.visible: false
                backButton.visible: true

                theme: Colors.currentTheme
                visible: true
            }
            PropertyChanges {
                target: applicationFlow
                previousState: applicationFlow.previousState
            }

        }

    ]
}
