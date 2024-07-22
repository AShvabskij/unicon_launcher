// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Effects

Item {
    id: root
    property alias greenBar: greenBar
    property alias pageText: caption.text
    property alias progressText: progressText.text

    property alias timer: timer

    property int scanTime: 3000

    property double progressBarValue: 0

    FontLoader {
        id: montserratFont
        source: "./fonts/montserrat/montserrat-latin-600-normal.ttf"
    }

    //! [Timer]
    Timer {
        id: timer
        interval: scanTime
        running: true
        repeat: true
        onTriggered: {
            root.parent.onProgressFinished()
        }
    }

    Timer {
        id: timer2
        interval: 100
        running: true
        onTriggered: {
            progressBarValue = 1
        }
    }

    Behavior on greenBar.width {
        SmoothedAnimation {
            easing.type: Easing.Linear
            velocity: (contentItem.width / scanTime) * 1000
        }
    }

    GridLayout {
        id: pGrid
        anchors.horizontalCenter: parent.horizontalCenter
//      anchors.verticalCenter: parent.verticalCenter
        anchors.top: parent.top
        anchors.topMargin: parent.height / 3.5
        flow: GridLayout.TopToBottom
        rowSpacing: 40

        Text {
            id: caption
            text: "Connecting to local device"
            font.pixelSize: 24
            font.family: montserratFont.name
            font.weight: 700
            color: Colors.currentTheme.textColor
            leftPadding: 20
        }
/*
        Rectangle {
            id: textDialog
            radius: 8
            Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
            gradient: Colors.greyBorder
            Layout.minimumHeight: 80
            Layout.minimumWidth: 350
            Rectangle {
                id: rectangle
                width: parent.width - 2
                height: parent.height - 2
                radius: 8
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                color: Colors.currentTheme.cardColor
                Text {
                    id: caption
                    text: "Connecting to local device"
                    color: Colors.currentTheme.textColor
                    font.pixelSize: 18
                    font.weight: 600
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
            MultiEffect {
                source: rectangle
                anchors.fill: rectangle
                shadowEnabled: true
                shadowColor: Colors.shadow
                shadowOpacity: 0.5
            }
        }
*/
        ProgressBar {
            id: control
            Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
            value: progressBarValue
            indeterminate: true
            Layout.topMargin: 20
            Layout.minimumWidth: 250
            implicitWidth: 350
//          implicitHeight: 35

            background: Rectangle {
                implicitHeight: 20
                color: "#e6e6e6"
            }

            contentItem: Item {
                id: contentItem
//              implicitWidth: dialog.width / 1.5
                implicitHeight: 4
                Rectangle {
                    id: greenBar
                    width: control.visualPosition * parent.width
                    height: parent.height
                    color: Colors.green
                }

                Image {
                    id: circle
                    source: (Colors.currentTheme == Colors.dark) ? "./images/icons/ellipse_dark.svg" : "./images/icons/ellipse_light.svg"
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.left
                }

                Image {
                    id: circle3
                    source: (Colors.currentTheme == Colors.dark) ? "./images/icons/ellipse_dark.svg" : "./images/icons/ellipse_light.svg"
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.right
                }

                Text {
                    id: progressText
                    color: Colors.green
                    text: qsTr("Loading...")
                    Layout.alignment: Qt.AlignHCenter
                    font.pixelSize: 14
                    font.weight: 600
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.top: greenBar.bottom
                    anchors.topMargin: 20
                }
            }
        }


    }
}
