pragma Singleton
import QtQuick 6.2

QtObject {
    readonly property int width: 740
    readonly property int height: Screen.desktopAvailableHeight * 0.96 < 850 ? Screen.desktopAvailableHeight*0.96 : 850 // Screen.desktopAvailableHeight * 0.8
    property bool isDarkTheme: true

    property string relativeFontDirectory: "fonts"

    /* Edit this comment to add your custom font */
    readonly property font font: Qt.font({
                                             family: Qt.application.font.family,
                                             pixelSize: Qt.application.font.pixelSize
                                         })
    readonly property font largeFont: Qt.font({
                                                  family: Qt.application.font.family,
                                                  pixelSize: Qt.application.font.pixelSize * 1.6
                                              })

    readonly property int fontCoeff: height < 850 ? 1 : 0 // Screen.desktopAvailableHeight * 0.8

    readonly property color accentColor: isDarkTheme ? "#002125" : "#DDE1E1"
    readonly property color primaryTextColor: isDarkTheme ? "#FFFFFF": "#000000"
    readonly property color secondaryTextColor: isDarkTheme ? "#D9D9D9" : "#000000"//"#c0c0c0": "#000000"
    readonly property color iconColor: isDarkTheme ? "#FFFFFF" : "#00414A"
    readonly property color iconColorWarn: isDarkTheme ? "#FF0010" : "#FF0010"

    readonly property color backgroundColor: "#EAEAEA"
}
