import QtQuick

Window {
    id: window
    visible: true
    title: qsTr("unicon_launcher")
    width: Constants.width
    height: Constants.height
    visibility: "Windowed"

    ApplicationFlow {
        width: parent.width
        height: parent.height
        mode: (Screen.height > Screen.width) ? "portrait" : "landscape"
    }

}
