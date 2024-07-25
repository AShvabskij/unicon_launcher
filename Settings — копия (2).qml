import QtQuick
import QtQuick.Layouts
import QtQuick.Controls 6.5

Item {
    id: root
    state: ""

    FontLoader {
        id: montserratFont
        source: "./fonts/montserrat/montserrat-latin-600-normal.ttf"
    }

    ColumnLayout {
        id: gridLayout
        width: parent.width
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        anchors.topMargin: 10

        Rectangle {
            id: infoPanel
            height: parent.height / 5
            color: "#1d1d1d"
            radius: 10
            border.color: "#3c3535"
            border.width: 2

            Layout.fillWidth: true
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
            Layout.rightMargin: 50
            Layout.leftMargin: 50

            Text {
                id: infoText
                color: "#dfdfdf"
                text: qsTr("This tool is designed for remote or local maintenance of highpower electronic devices. The tool allows you to connect to the web interface of the selected device to monitor the device’s performance parameters and usage statistics./n To run the To receive license Please generate application")
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                font.pixelSize: 16
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignTop
                wrapMode: Text.Wrap
                anchors.topMargin: 18
                anchors.bottomMargin: 18
                anchors.leftMargin: 24
                anchors.rightMargin: 24
                font.weight: Font.Normal
                font.family: montserratFont.name
            }
        }

        Item {
            id: appKey
            Layout.fillHeight: false
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
            Layout.rightMargin: 50
            Layout.leftMargin: 50
            Layout.topMargin: 50

            ColumnLayout {
                width: parent.width

                Text {
                    id: appKeyText
                    text: qsTr("Application key:")
                    font.pixelSize: 18
                    font.weight: Font.Normal
                    font.family: montserratFont.name
                    color: "#dfdfdf"
                    Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                }

                TextField {
                    id: appKeyInput
                    width: parent.width
                    Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                    height: 40
                    font.pixelSize: 18
                    font.weight: 400
                    font.family: montserratFont.name
                    color: "#dfdfdf"
                    verticalAlignment: Text.AlignVCenter
                    leftPadding: 10
                    placeholderText: "APP Key"
                    placeholderTextColor: "#787878"
                    text: "Fs5WF+c1G0Bh2x/ur33wmw33zY3Id9FR/mQQc7XaiSA="
                    background: Rectangle {
                        color: "#1d1d1d"
                        border.color: "#3c3535"
                    }
                }

                RowLayout {
                    anchors.top: appKeyInput.bottom
                    anchors.left: appKeyInput.left
                    anchors.leftMargin: 10
                    anchors.topMargin: 10
                    spacing: 20
                    Layout.alignment: Qt.AlignLeft | Qt.AlignTop

                    CustomButton {
                        id: generateButton
                        Layout.minimumWidth: 150
                        Layout.minimumHeight: 40
                        buttonText: "Generate"
                        showIcon: false
                        fontSize: 18
                    }

                    CustomButton {
                        id: copyButton
                        Layout.minimumWidth: 150
                        Layout.minimumHeight: 40
                        buttonText: "Copy"
                        showIcon: false
                        fontSize: 18
                    }
                }
            }
        }

        Item {
            id: license
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
            Layout.rightMargin: 50
            Layout.leftMargin: 50

            Text {
                id: licenseText
                text: qsTr("License number:")
                font.pixelSize: 18
                font.weight: Font.Normal
                font.family: montserratFont.name
                color: "#dfdfdf"
            }
        }
    }
}
