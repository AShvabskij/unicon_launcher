import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Dialogs
import qmlmodels 1.0

RowLayout {

    property alias label: label
    property alias input: input

    Layout.topMargin: 0
    Layout.rightMargin: 10


    Text {
        id: label
        text: ""
        font.pixelSize: 16 - Constants.fontCoeff
        font.weight: Font.Normal
        font.family: montserratFont.name
        color: Colors.currentTheme.textColor
        Layout.alignment: Qt.AlignLeft | Qt.AlignTop
        Layout.leftMargin: 10
        Layout.preferredWidth: 100 // Set a fixed width for the label to align both fields
    }

    TextField {
        id: input
        Layout.fillWidth: true
        Layout.minimumWidth: 100 // Minimum width it can shrink to
        Layout.alignment: Qt.AlignLeft | Qt.AlignTop
        leftPadding: 10

        font.pixelSize: 16 - Constants.fontCoeff
        font.weight: 400
        font.family: montserratFont.name
        color: Colors.currentTheme.textColor

        verticalAlignment: Text.AlignVCenter
        text: ""

        background: Rectangle {
            color: Colors.currentTheme.boxColor
            border.color: Colors.currentTheme.borderColor
        }
    }
}
