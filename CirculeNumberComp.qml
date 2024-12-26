import QtQuick
import QtQuick.Controls

Item {
    id: circleWithNumber
    width: 100
    height: 100

    // The white circle
    Rectangle {
        id: circle
        width: parent.width
        height: parent.height
        radius: width / 2
        color: "transparent"
        border.color: "#3E3E3E"
        border.width: 2
        anchors.centerIn: parent
    }

    // The number inside the circle
    Text {
        id: numberText
        text: ""
        font.pixelSize: circle.width * 0.4 // Scaled relative to the circle size
        anchors.centerIn: circle
        font.weight: 600
        font.family: "Titillium Web"
        color: activated ? Constants.primaryTextColor : "#898989"
    }

    // Properties to customize the number
    property alias number: numberText.text
    property alias circleSize: circle.width
}
