// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Effects
import QtWebView

Item {
    id: remoteNode
    property string nodeUrl: "http://localhost:3000"
    property string nodeName: ""
    state: "progress"

    function onProgressFinished() {
        state = "content"
        applicationFlow.toogleToFullScreen()
    }

    states: [
        State {
            name: "content"
            PropertyChanges {
                target: progressItem
                visible: false
            }
            PropertyChanges {
                target: webView
                visible: true
            }
        }
     ]

    ProgressItem {
        id: progressItem
        scanTime: 1000
        pageText: "Connecting to remote Node " + nodeName
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter

//      anchors.topMargin: parent.height / 40
        anchors.top: remoteNode.top
        visible: true
    }

    WebView {
        id: webView
        url: initialUrl
        visible: false
/*
        Layout.preferredHeight: parent.height
        Layout.preferredWidth: parent.width
        Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
        Layout.margins: 5
*/
//      anchors.horizontalCenter: parent.horizontalCenter

        anchors.top: remoteNode.top
        width: parent.width
        height: parent.height

       onLoadingChanged: function(loadRequest) {
            if (loadRequest.errorString)
                console.error(loadRequest.errorString);
        }

        Component.onCompleted: {
            settings.javaScriptEnabled = true
            settings.localStorageEnabled = true
            settings.allowFileAccess = true
            settings.localContentCanAccessFileUrls = true
        }
    }
}
