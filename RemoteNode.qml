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
    property string nodeRoute: ""
    property string nodeUrl: "http://" + nodeIP + ":" + nodePort + "/" + nodeRoute
    property string nodePort: "3000"

    property string nodeName: ""
    state: "progress"
    property int progressValue: 0
    property int refreshCount: 0

    function onProgressFinished() {
    }

    function onRefreshPage() {
        if (webView && webView.visible === true) {
            refreshCount++;
            if (refreshCount >= 5) {
                webView.profile.clearHttpCache();
                refreshCount = 0;
            } else {
                timerRefreshCount.restart()
            }

            webView.reload()
        }
    }

    Timer {
        id: timerRefreshCount
        interval: 1000 * 30
        repeat: false
        running: false
        onTriggered: {
            refreshCount = 0;
        }
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
        visible: true

        anchors.top: remoteNode.top
        width: parent.width
        height: parent.height

       onLoadingChanged: function(loadRequest) {
            if (loadRequest.errorString && nodeUrl !== "") {
                console.error("Failed to open " + nodeUrl + ", error = " + loadRequest.errorString);
            }
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
            cachePath: "./webcache"
     }

    Timer {
        id: timerOnCompleted
        interval: 1000
        repeat: false
        running: false
        onTriggered: {
            remoteNode.state = "content"
            applicationFlow.toggleToFullScreen();
            applicationFlow.onWebOpened();
        }
    }

    Timer {
        id: timerOnProgress
        interval: 100
        repeat: true
        running: true
        onTriggered: {
            if (progressValue >= webView.loadProgress) {
                progressValue = progressValue < 97 ? progressValue + 1 : 97
            } else {
                progressValue = webView.loadProgress
            }

            progressItem.progressBarValue = progressValue / 100

            if (progressValue > 80 && progressValue <= 90) {
                interval = 1000;
            }

            if (progressValue > 90) {
                interval = 3000;
            }


            progressItem.progressText = "Loading... " + progressValue + "%"

            if (webView.loadProgress === 100 && webView.loading === false) {
                timerOnProgress.running = false;
                timerOnCompleted.running = true;
            }
        }
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

        onRenderProcessTerminated: {
            console.error("render proccess terminated")
        }

        onFileDialogRequested: {
            console.info("File dialog requested")
        }

        onLoadProgressChanged: {
            if (loadProgress === 0) {
                console.info("loading of the web page started")
            }

            if (loadProgress === 100) {
                console.info("Loading of the web page completed");
            }
        }

        onLoadingChanged: function(loadRequest) {
             if (loadRequest.errorString && nodeUrl !== "")
                 if (!loadRequest.errorString.includes('failedUrl')) {
                     console.error("Failed to open " + nodeUrl + ", error = " + loadRequest.errorString);
                 }
        }

        Component.onCompleted: {
        }
    }
}
