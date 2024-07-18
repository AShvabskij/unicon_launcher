import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Effects
import QtQuick.Dialogs

Dialog {
    id: root
    anchors.centerIn: parent

    title: "warning"
    property string text: ""
    property string informativeText: ""

    implicitWidth: 350
    implicitHeight: 200
    padding: 0
    // standardButtons: MessageDialog.Ok
    visible: false
    modal: true

    header:
        Label {
        text: root.title
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        color: "#ffffff"
        font.pointSize: 10
        font.bold: true
        padding: 0
        height: 30

        background: Rectangle {
            color: "#353535"
            border.width: 1.0
            border.color: Colors.border
        }
    }

    // Rectangle {

    //     border.width: 1
    //     border.color: Colors.border
    //     color: Colors.currentTheme == Colors.dark ? Colors.dark.borderColor : Colors.light.borderColor
    //     height: 30
    // }

    contentItem:  Label {
        text: root.text
        font.pointSize: 12
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        color: "#ffffff"
        font.bold: true
        padding: 0
        background: Rectangle {
            color: "#353535"
            border.width: 1.0
            border.color: Colors.border
        }
    }

    footer: DialogButtonBox {
        background: Rectangle {
            color: "#353535"
            border.width: 1.0
            border.color: Colors.border
        }

        AbstractButton {
            DialogButtonBox.buttonRole: DialogButtonBox.AcceptRole
            implicitWidth: 100
            height: parent.height
            padding: 5

            contentItem: Text {
                text: qsTr("Ok")
                color: "#ffffff"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.pointSize: 10
                font.bold: true
            }
            background: Rectangle {
                color: "#707070"
                radius: 1
                border.width: 1.0
                border.color: Colors.border
            }
        }
    }
}
