import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Dialogs
import qmlmodels 1.0

RowLayout {

    property alias label: label
    property alias text: text

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

    Text {
        id: text
        text: ""
        font.pixelSize: 16 - Constants.fontCoeff
        font.weight: Font.Normal
        font.family: montserratFont.name
        color: Colors.currentTheme.textColor
        Layout.alignment: Qt.AlignLeft | Qt.AlignTop
        Layout.leftMargin: 10
        Layout.preferredWidth: 100 // Set a fixed width for the label to align both fields
    }
}
