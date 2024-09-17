import QtQuick

Window {
    id: window
    visible: true
    title: qsTr("IRST")
    width: Constants.scaledWidth
    height: Constants.scaledHeight
    visibility: "Windowed"
//  flags: Qt.FramelessWindowHint

    ApplicationFlow {
        width: parent.width
        height: parent.height
        mode: (Screen.height > Screen.width) ? "portrait" : "landscape"
    }
}
