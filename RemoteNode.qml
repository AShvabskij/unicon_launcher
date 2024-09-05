// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Effects
import QtWebView
import QtWebEngine

Item {
    id: remoteNode
    property string nodeIP: "localhost"
    property string nodeUrl: "http://" + nodeIP + ":" + nodePort
    property string nodePort: "3000"

    property string nodeName: ""
    state: "progress"

    function onProgressFinished() {
        state = "content"
        applicationFlow.toogleToFullScreen();
        applicationFlow.onWebOpened();
    }

    function onRefreshPage() {
        webView.reload()
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
        scanTime: 3000
        pageText: "Connecting to " + nodeName
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        progressText: "Loading..."

        anchors.top: remoteNode.top
        visible: true
    }
/*
    WebView {
        id: webView
        url: nodeUrl
        visible: false

        // Layout.preferredHeight: parent.height
        // Layout.preferredWidth: parent.width
        // Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
        // Layout.margins: 5

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
*/

    property QtObject defaultProfile: WebEngineProfile {
            storageName: "Profile"
            offTheRecord: false
            httpCacheType : WebEngineProfile.DiskHttpCache
     }

    WebEngineView {
        id: webView
        anchors.top: remoteNode.top
        anchors.topMargin: 10
        width: parent.width
        height: parent.height

        url: nodeUrl
        visible: false

        settings.allowRunningInsecureContent: true
        settings.allowWindowActivationFromJavaScript: true
        settings.autoLoadIconsForPage: true
        settings.autoLoadImages: true
        settings.errorPageEnabled: true
        settings.fullScreenSupportEnabled: true
        settings.javascriptCanOpenWindows: true
        settings.localStorageEnabled: true

        profile : defaultProfile

        // profile.offTheRecord: false
        // profile.httpCacheType : WebEngineProfile.DiskHttpCache
        // profile.cachePath: "web_cache"
        // profile.storageName: "web_storage"


        onLoadingChanged: function(loadRequest) {
             if (loadRequest.errorString)
                 console.error(loadRequest.errorString);
        }
    }
}
