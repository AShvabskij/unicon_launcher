import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects

AbstractButton {
    // Height, width and any other size related properties containing odd looking float or other dividers
    // that do not seem to have any logical origin are just arbitrary and based on original design
    // and/or personal preference on what looks nice.
    id: root

    required property bool active
    required property string name
    required property string region
    required property string owner
    required property string address

    required property string iconName
    required property string lastAccessTime
    required property string workTime
    required property string startNumber
    required property string status
    required property string ipAddress

    property bool isSmallLayout: false

    states: State {
        name: "pressed"
        when: root.pressed
        PropertyChanges {
            target: card
            scale: 0.99
        }
    }

    transitions: Transition {
        NumberAnimation {
            properties: "scale"
            duration: 10
            easing.type: Easing.InOutQuad
        }
    }
/*
    topPadding: 12
    leftPadding: 12
    bottomPadding: 0
    rightPadding: 16
*/
    contentItem: Rectangle {
        id: card

        radius: 12
        color: Constants.accentColor
        border.color: "#3E3E3E"
        border.width: 2

        scale: 1

        ColumnLayout {
            anchors.top: card.top
            anchors.topMargin: 12

            RowLayout {
                id: header

                Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                Layout.leftMargin: internal.columnMargin
                Layout.rightMargin: internal.columnMargin
/*
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.rightMargin: internal.columnMargin
                anchors.leftMargin: internal.columnMargin
*/
                spacing: 16

                Item {
                    Layout.preferredWidth: internal.iconSize
                    Layout.preferredHeight: internal.iconSize
                    Layout.alignment: Qt.AlignTop

                    Image {
                        id: icon

                        source: "./images/icons/" + root.iconName
                        sourceSize.width: internal.iconSize
                        sourceSize.height: internal.iconSize
                    }

                    MultiEffect {
                        anchors.fill: icon
                        source: icon
                        colorization: 1
                        colorizationColor: Constants.iconColor
                    }
                }

                Column {
                    id: title

                    Layout.fillWidth: true
                    spacing: internal.titleSpacing

                    Label {
                        text: root.region + " " + root.name
                        font.pixelSize: !root.isSmallLayout ? 20 : 16
                        font.weight: 600
                        font.family: "Titillium Web"
                        color: active ? Constants.primaryTextColor : "#898989"
                    }

                    Label {
                        text: root.address
                        font.pixelSize: 12
                        font.weight: 400
                        font.family: "Titillium Web"
                        color:active ? Constants.primaryTextColor : "#898989"
                    }

                    Label {
                        text: root.owner
                        font.pixelSize: 12
                        font.weight: 400
                        font.family: "Titillium Web"
                        color: active ? Constants.primaryTextColor : "#898989"
                    }
                }
            }

            Column {
                id: propsColumn

                spacing: internal.spacing
                Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                Layout.leftMargin: internal.columnMargin
                Layout.topMargin: !root.isSmallLayout ? 20 : 16

                Repeater {
                    model: [qsTr("Last access date: %1".arg(lastAccessTime)), qsTr("Work time: %1").arg(workTime), qsTr("Number of starts: %1").arg(startNumber)]

                    Label {
                        text: modelData
                        font.pixelSize: !root.isSmallLayout ? 14 : 12
                        font.weight: 400
                        font.family: "Titillium Web"
                        color: active ? Constants.secondaryTextColor : "#898989"
                    }
                }
            }

            RowLayout {
                id: statusRow
                Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                Layout.leftMargin: internal.columnMargin
                Layout.topMargin: 12

                // anchors.left: parent.left
                // anchors.right: parent.right
                // anchors.top: propsColumn.bottom
                // anchors.leftMargin: internal.columnMargin
                // anchors.topMargin: 12

                spacing: 12

                Item {
                    Layout.preferredWidth: internal.iconSize /2
                    Layout.preferredHeight: internal.iconSize /2
                    Layout.alignment: Qt.AlignTop
                    visible: status != "" ? true : false
                    Image {
                        id: iconFlag

                        source: "./images/icons/flag.svg"
                        sourceSize.width: internal.iconSize/2
                        sourceSize.height: internal.iconSize/2
                    }

                    MultiEffect {
                        anchors.fill: iconFlag
                        source: iconFlag
                        colorization: 1
                        colorizationColor: Constants.iconColorWarn
                    }
                }


                Label {
                    text: status
                    visible: status != "" ? true : false
                    font.pixelSize: !root.isSmallLayout ? 14 : 12
                    font.weight: 400
                    font.family: "Titillium Web"
                    color: Constants.iconColorWarn
                }

            }
        }
}
        QtObject {
            id: internal

            property int rightMargin: 60
            property int leftPadding: 16
            property int titleSpacing: 8
            property int spacing: 4
            property int columnMargin: 16
            property int iconSize: 34
            property int switchMargin: 9
        }
/*
        MouseArea {
            anchors.fill: parent
            onClicked: root.scale = 0.9
        }
*/


}

