// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause
import QtQuick
import QtQuick.Layouts

Item {
    id: home

    property alias localButton: localButton
    property alias remoteButton: remoteButton

    property alias grid: grid
    property alias header: header

    FontLoader {
        id: montserratFont
        source: "./fonts/montserrat/montserrat-regular.ttf"
    }

    FontLoader {
        id: montserratThinFont
        source: "./fonts/montserrat/montserrat-thin.ttf"
    }

    states: [
        State {
            name: "portrait"
            PropertyChanges {
                target: localButton
                Layout.preferredWidth: home.width / 2.2
                Layout.preferredHeight: home.height / 14
                Layout.topMargin: 20
            }
        },
        State {
            name: "landscape"
            PropertyChanges {
                target: grid
                anchors.topMargin: 0
            }
            PropertyChanges {
                target: header
                Layout.topMargin: 0
            }
            PropertyChanges {
                target: localButton
                Layout.preferredWidth: home.width / 3
                Layout.preferredHeight: home.height / 8
                Layout.topMargin: 0
            }
            PropertyChanges {
                target: remoteButton
                Layout.preferredWidth: home.width / 3
                Layout.preferredHeight: home.height / 8
                Layout.topMargin: 0
            }
            PropertyChanges {
                target: dashButton
                Layout.preferredWidth: home.width / 3
                Layout.preferredHeight: home.height / 8
                Layout.topMargin: 0
            }

        }
    ]

    GridLayout {
        id: grid
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: home.top
        flow: GridLayout.TopToBottom
        rowSpacing: 30
        Image {
            id: image
            Layout.preferredHeight: home.height / 3
            Layout.preferredWidth: height / 1.16
            Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
//            Layout.margins: 5
            source: "./images/icons/home.svg"
        }
        Text {
            id: header
            text: qsTr("REMOTE SUPPORT TOOL")
            font.pixelSize: 24
//            font.weight: 700
            font.bold: true
            font.family: montserratThinFont.name
            color: Colors.currentTheme.textColor
            Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
            Layout.topMargin: 20
        }

        // Text {
        //     id: caption
        //     text: qsTr(" Support Devices Localy or Remotely ")
        //     font.family: montserratFont.name
        //     font.pixelSize: 16
        //     horizontalAlignment: Text.AlignHCenter
        //     Layout.maximumWidth: home.width
        //     wrapMode: Text.Wrap
        //     font.weight: 400
        //     color: Colors.currentTheme.caption
        //     Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
        // }

        CustomButton {
            id: localButton
            Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
            Layout.minimumWidth: 190
            Layout.minimumHeight: 50
            Layout.topMargin: 50
            fontSize: 24

            buttonText: "LOCAL"
            showIcon: false
        }
        CustomButton {
            id: remoteButton
            Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
            Layout.minimumWidth: 190
            Layout.minimumHeight: 50
            fontSize: 24

            buttonText: "REMOTE"
            showIcon: false
        }
        CustomButton {
            id: dashButton
            Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
            Layout.minimumWidth: 190
            Layout.minimumHeight: 50
            fontSize: 24

            enabled: false
            buttonText: "DASHBOARD"
            showIcon: false
        }
    }
}
