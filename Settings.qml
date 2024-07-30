import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Dialogs
import qmlmodels 1.0

Item {
    id: root
    state: ""
    property bool licenseValidity: licenseModel.isValid

    FontLoader {
        id: montserratFont
        source: "./fonts/montserrat/montserrat-latin-600-normal.ttf"
    }

    FileDialog {
        id: fileDialog
        title: "Select a .dat file"
        currentFolder: StandardPaths.standardLocations(StandardPaths.DownloadLocation)[0]
        nameFilters: ["*.dat"]
        onAccepted: {
            console.log("Selected file: " + selectedFile)
            licenseModel.saveLicense(selectedFile)
            licenseModel.parseLicense()
        }
        onRejected: {
            console.log("File selection was canceled")
        }
    }

    LicenseModel {
        id: licenseModel
        onErrorRaised: {
            root.state = "error"
            msgError.text = error
            msgError.informativeText = informativeError
            msgError.visible = true
        }

        Component.onCompleted: {
            licenseModel.parseLicense()
        }
    }

    WarningBox {
        id: msgError
        onAccepted: applicationFlow.backButton()
    }

    ColumnLayout {
        id: gridLayout
        width: parent.width
        anchors.top: parent.top
        anchors.topMargin: 10

        Rectangle {
            id: infoPanel
            height: 210
            color: Colors.currentTheme.boxColor
            radius: 10
            border.color: Colors.currentTheme.borderColor
            border.width: 2

            Layout.fillWidth: true
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
            Layout.rightMargin: 50
            Layout.leftMargin: 50

            Text {
                id: infoText
                color: Colors.currentTheme.textColor
                textFormat: Text.RichText
                text: qsTr(" This tool is designed for remote or local maintenance of highpower electronic devices. The tool allows you to connect to the web interface of the selected device to monitor the device’s performance parameters and usage statistics.")
                      + "<br>" + qsTr(` To activate a remote support tool please load a license file, requested from the support team`)
                      + "<br><br>" + qsTr(` Tech Support: <b>techsupport@solconigel.com`) + "</b>"
                      + "<br>" + `Visit:  <a href="https://solconigel.com/">SolconIgel</a> for more information`
                onLinkActivated: {
                    Qt.openUrlExternally(link);
                }

                anchors.fill: parent
                anchors.topMargin: 18
                anchors.bottomMargin: 18
                anchors.leftMargin: 24
                anchors.rightMargin: 24
                font.pixelSize: 16
                font.weight: Font.Normal
                font.family: "Titillium Web"
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignTop
                wrapMode: Text.Wrap
            }
        }

        Rectangle {
            id: appKey
            height: appKeyContent.height + 30

            Layout.fillWidth: true
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
            Layout.rightMargin: 50
            Layout.leftMargin: 50
            Layout.topMargin: 30
            color: "transparent"
            radius: 10
            border.color: Colors.currentTheme.borderColor//"#3c3535"
            border.width: 0

            ColumnLayout {
                id: appKeyContent
                width: parent.width

                Text {
                    id: appKeyText
                    text: qsTr("Application key:")
                    font.pixelSize: 18
                    font.weight: Font.Normal
                    font.family: montserratFont.name
                    color: Colors.currentTheme.textColor //"#dfdfdf"
                    Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                    Layout.rightMargin: 10
                    Layout.leftMargin: 10
                    Layout.topMargin: 10
                }

                Text {
                    id: helpAppKeyText
                    color: Colors.currentTheme.helpTextColor
                    text: qsTr("Please generate your APP key and send it to the support team (techsupport@solconigel.com)")
                    Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                    Layout.leftMargin: 10
                    Layout.topMargin: 0

                    font.pixelSize: 13
                    font.family: montserratFont.name
                }

                TextField {
                    id: appKeyInput
                    Layout.rightMargin: 10
                    Layout.leftMargin: 10
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                    font.pixelSize: 18
                    font.weight: 400
                    font.family: montserratFont.name
                    color: Colors.currentTheme.textColor
                    verticalAlignment: Text.AlignVCenter
                    leftPadding: 10
                    placeholderText: "APP Key"
                    placeholderTextColor: Colors.currentTheme.helpTextColor
                    text: licenseModel.appKey
                    selectByMouse: true
                    background: Rectangle {
                        color: Colors.currentTheme.boxColor
                        border.color: Colors.currentTheme.borderColor
                    }
                }

                RowLayout {
                    anchors.top: appKeyInput.bottom
                    anchors.left: appKeyInput.left
                    anchors.leftMargin: 0
                    anchors.topMargin: 10
                    spacing: 20
                    Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                    Layout.minimumHeight: 40

                    CustomButton {
                        id: generateButton
                        Layout.minimumWidth: 150
                        Layout.minimumHeight: 50
                        buttonText: "Generate"
                        showIcon: false
                        fontSize: 18
                        enabled: !licenseModel.isValid

                        onClicked: {
                            licenseModel.generateKey();
                        }
                    }

                    CustomButton {
                        id: copyButton
                        Layout.minimumWidth: 150
                        Layout.minimumHeight: 50
                        buttonText: "Copy"
                        showIcon: false
                        fontSize: 18
                        onClicked: {
                            appKeyInput.selectAll()
                            appKeyInput.copy()
                        }
                    }
                }
            }
        }

        Rectangle {
            id: license
            height: licenseContent.height + 30
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
            Layout.rightMargin: 50
            Layout.leftMargin: 50
            Layout.topMargin: 10
            color: "transparent"
            radius: 10
            border.color: Colors.currentTheme.borderColor
            border.width: 0

            ColumnLayout {
                id: licenseContent
                width: parent.width
                Text {
                    id: licenseText
                    text: qsTr("License number:")
                    font.pixelSize: 18
                    font.weight: Font.Normal
                    font.family: montserratFont.name
                    color: Colors.currentTheme.textColor //"#dfdfdf"
                    Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                    Layout.rightMargin: 10
                    Layout.leftMargin: 10
                    Layout.topMargin: 10
                }

                TextField {
                    id: licenseInput
                    Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                    Layout.rightMargin: 10
                    Layout.leftMargin: 10
                    Layout.fillWidth: true
                    font.pixelSize: 18
                    font.weight: 400
                    font.family: montserratFont.name
                    color: Colors.currentTheme.textColor //"#dfdfdf"
                    verticalAlignment: Text.AlignVCenter
                    leftPadding: 10
                    placeholderText: ""
                    readOnly: true
                    placeholderTextColor: Colors.currentTheme.helpTextColor
                    text: licenseModel.licenseNumber
                    background: Rectangle {
                        color: Colors.currentTheme.boxColor
                        border.color: Colors.currentTheme.borderColor
                    }
                }

                Text {
                    id: statusText
                    color: licenseValidity ? "#227102" : "#CC0000"
                    text: licenseValidity ? qsTr("License activated successfully") : qsTr("Invalid license")
                    anchors.left: licenseText.left

                    font.pixelSize: 15
                    anchors.leftMargin: 00
                    anchors.topMargin: 6
                    font.family:  montserratFont.name
                    visible: licenseModel.isExists

                }

                Text {
                    id: helpLicenseText
                    color: Colors.currentTheme.helpTextColor
                    text: qsTr("Please select the license file received from the support team")
                    anchors.left: licenseInput.left
                    anchors.top: statusText.visible ? statusText.bottom : licenseInput.bottom
                    font.pixelSize: 13
                    anchors.leftMargin: 0
                    anchors.topMargin: 20
                    font.family: montserratFont.name
                }

                RowLayout {
                    anchors.top: helpLicenseText.bottom
                    anchors.left: helpLicenseText.left
                    anchors.leftMargin: 00
                    anchors.topMargin: 10
                    spacing: 20
                    Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                    Layout.minimumHeight: 40

                    CustomButton {
                        anchors.topMargin: 50
                        id: licenseButton
                        Layout.minimumWidth: 230
                        Layout.minimumHeight: 50
                        buttonText: "Select license"
                        showIcon: true
                        fontSize: 18
                        onClicked: {
                            fileDialog.open();
                        }
                    }

                    CustomButton {
                        anchors.topMargin: 50
                        id: licenseButtonRemove
                        Layout.minimumWidth: 230
                        Layout.minimumHeight: 50
                        buttonText: "Delete license"
                        showIcon: false
                        fontSize: 18
                        enabled: licenseModel.isExists
                        onClicked: {
                            licenseModel.deleteLicense()
                        }
                    }
                }
            }
        }

    }
}
