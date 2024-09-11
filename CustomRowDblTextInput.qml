import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Dialogs
import qmlmodels 1.0

RowLayout {

    property alias label1: label1.text
    property alias input1: input1.text
    property int   inputWidth1: 150

    property alias label2: label2.text
    property alias input2: input2.text

    Layout.topMargin: 0
    Layout.rightMargin: 10


    Text {
        id: label1
        text: ""
        font.pixelSize: 16
        font.weight: Font.Normal
        font.family: montserratFont.name
        color: Colors.currentTheme.textColor
        Layout.alignment: Qt.AlignLeft | Qt.AlignTop
        Layout.leftMargin: 10
        Layout.preferredWidth: 100 // Set a fixed width for the label to align both fields
    }

    TextField {
        id: input1
        Layout.fillWidth: true
        Layout.minimumWidth: 100 // Minimum width it can shrink to
        Layout.alignment: Qt.AlignLeft | Qt.AlignTop
        Layout.preferredWidth: inputWidth // A reasonable width

        leftPadding: 10

        font.pixelSize: 16
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

    Text {
        id: label2
        text: ""
        font.pixelSize: 16
        font.weight: Font.Normal
        font.family: montserratFont.name
        color: Colors.currentTheme.textColor
        Layout.alignment: Qt.AlignLeft | Qt.AlignTop
        Layout.leftMargin: 10
    }

    TextField {
        id: input2
        Layout.fillWidth: true
        Layout.alignment: Qt.AlignLeft | Qt.AlignTop
        leftPadding: 10

        font.pixelSize: 16
        font.weight: 400
        font.family: montserratFont.name
        color: Colors.currentTheme.textColor

        verticalAlignment: Text.AlignVCenter
        text: ""
        Layout.minimumWidth: 100 // Minimum width it can shrink to
        background: Rectangle {
            color: Colors.currentTheme.boxColor
            border.color: Colors.currentTheme.borderColor
        }
    }

}
