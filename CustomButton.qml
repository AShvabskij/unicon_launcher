// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause
import QtQuick

CustomButtonForm {
    rectangle.gradient: if (Colors.currentTheme == Colors.dark) {
                            Colors.darkButtonGradient
                        } else if (Colors.currentTheme == Colors.light) {
                            Colors.lightButtonGradient
                        } else {
                            Colors.greenButtonGradient
                        }
}
