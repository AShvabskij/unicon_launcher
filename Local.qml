// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Effects
import QtQuick.Dialogs
import QtWebView

import qmlmodels 1.0

Item {
    id: localPage
    property string initialUrl: "" //"http://localhost:3000"
    state: "progress"

    function onProgressFinished() {
        progressItem.progressBarValue = progressItem.progressBarValue === 0 ? 1 : 0;
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
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        pageText: "ESTABLISHING DIRECT CONNECTION"
        progressText: "searching devices..."

//      anchors.topMargin: parent.height / 40
        anchors.top: localPage.top
        visible: true
    }

    LocalModel {
        id: localModel
        onConnected:  {
            state = "content"
            applicationFlow.toogleToFullScreen();
            applicationFlow.onWebOpened();
            initialUrl = localModel.getURL();
        }

        onErrorRaised: function(errorTxt, informativeError) {
            state = "error"
            msgError.text = errorTxt
            msgError.informativeText = informativeError
            msgError.visible = true
        }
    }

    WarningBox {
        id: msgError
        onAccepted: applicationFlow.backButton()
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

        anchors.top: localPage.top
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

    Component.onCompleted: {
        localModel.scanIP()
    }

}
