// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause
import QtQuick

CustomToolBarForm {
    backButton.onClicked: applicationFlow.backButton()
    themeButton.onClicked: applicationFlow.themeButton()
    settingsButton.onClicked: applicationFlow.settingsButton()
    aboutButton.onClicked: applicationFlow.aboutButton()
    refreshButton.onClicked: applicationFlow.refreshButton()

    backButton.states: State {
        name: "pressed"
        when: backButton.pressed
        PropertyChanges {
            target: backButton
            scale: 1.4
        }
    }
    themeButton.states: State {
        name: "pressed"
        when: themeButton.pressed
        PropertyChanges {
            target: themeButton
            scale: 1.4
        }
    }
    settingsButton.states: State {
        name: "pressed"
        when: settingsButton.pressed
        PropertyChanges {
            target: settingsButton
            scale: 1.4
        }
    }

    aboutButton.states: State {
        name: "pressed"
        when: aboutButton.pressed
        PropertyChanges {
            target: aboutButton
            scale: 1.4
        }
    }

    refreshButton.states: State {
        name: "pressed"
        when: refreshButton.pressed
        PropertyChanges {
            target: refreshButton
            scale: 1.4
        }
    }

}
