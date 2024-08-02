import QtQuick

Window {
    id: window
    visible: true
    title: qsTr("IRST")
    width: Constants.width
    height: Constants.height
    visibility: "Windowed"
//  flags: Qt.FramelessWindowHint

    ApplicationFlow {
        width: parent.width
        height: parent.height
        mode: (Screen.height > Screen.width) ? "portrait" : "landscape"
    }
}
