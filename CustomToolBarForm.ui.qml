// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Rectangle {
    // Height, width and any other size related properties containing odd looking float or other dividers
    // that do not seem to have any logical origin are just arbitrary and based on original design
    // and/or personal preference on what looks nice.
    id: root
    property alias backButton: backButton
    property alias themeButton: themeButton
    property alias settingsButton: settingsButton
    property alias aboutButton: aboutButton
    property alias refreshButton: refreshButton
    property alias demoButton: demoButton

    property var theme: Colors.dark

    color: (theme == Colors.dark) ? "#1e1e1e" : "#e5e5e5"
    border.color: (theme == Colors.dark) ? "#2c2c2c" : "#FFFFFF"

    RowLayout {
        width: parent.width

        Row {
            Layout.alignment: Qt.AlignLeft
            spacing: 5

            Button {
                id: backButton
                icon.color: theme.iconColor //(theme == Colors.dark) ? "#FFFFFF" : "#121111"
                icon.source: "./images/icons/keyboard_backspace_black_left.svg"
                flat: false
                background: null
                transitions: Transition {
                    NumberAnimation {
                        properties: "scale"
                        duration: 100
                        easing.type: Easing.InOutQuad
                    }
                }
            }
            Button {
                id: refreshButton
                icon.color: theme.iconColor
                icon.source: "./images/icons/refresh.svg"
                flat: true
                background: null
                transitions: Transition {
                    NumberAnimation {
                        properties: "scale"
                        duration: 100
                        easing.type: Easing.InOutQuad
                    }
                }

            }

            Button {
                id: demoButton
                icon.color: theme.iconColor
                icon.source: "./images/icons/demo.svg"
                flat: true
                background: null
                checkable: true
//                display: AbstractButton.TextOnly
//                text: " D "
//              font.bold: true
//              font.pixelSize: 16
                transitions: Transition {
                    NumberAnimation {
                        properties: "scale"
                        duration: 100
                        easing.type: Easing.InOutQuad
                    }
                }
            }

        }

        Row {
            Layout.alignment: Qt.AlignRight

            Button {
                id: settingsButton
                icon.color: theme.iconColor
                icon.source: "./images/icons/license.svg"
                flat: true
                background: null
                transitions: Transition {
                    NumberAnimation {
                        properties: "scale"
                        duration: 100
                        easing.type: Easing.InOutQuad
                    }
                }

            }

            Button {
                id: aboutButton
                icon.color: theme.iconColor
                icon.source: "./images/icons/info.svg"
                flat: true
                background: null
                transitions: Transition {
                    NumberAnimation {
                        properties: "scale"
                        duration: 100
                        easing.type: Easing.InOutQuad
                    }
                }
            }

            Button {
                id: themeButton
                icon.color: theme.iconColor
                icon.source: (theme == Colors.dark) ? "./images/icons/light_mode_black_24dp.svg" : "./images/icons/dark_mode_black_24dp.svg"
                flat: true
                background: null
                transitions: Transition {
                    NumberAnimation {
                        properties: "scale"
                        duration: 100
                        easing.type: Easing.InOutQuad
                    }
                }
            }
        }
    }
}
