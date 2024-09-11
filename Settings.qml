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
        source: "./fonts/montserrat/montserrat-regular.ttf"
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
        anchors.topMargin: 25
        /*
        Rectangle {
            id: infoPanel
            height: 150
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
                text: qsTr(" This tool enables you to connect to your selected device's web interface, allowing you to monitor its performance parameters and usage statistics")
                      + "<br><br>" + `Visit:  <a href="https://solconigel.com/">Solcon-IGEL Group</a> for more information`
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
                font.family: montserratFont.name
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignTop
                wrapMode: Text.Wrap
            }
        }
*/
        Rectangle {
            id: userInfo
            height: userInfoContent.height + 30

            Layout.fillWidth: true
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
            Layout.rightMargin: 50
            Layout.leftMargin: 50
            Layout.topMargin: 10
            Layout.minimumWidth: 500 // Minimum width it can shrink to

            color: "transparent"
            radius: 10
            border.color: Colors.currentTheme.borderColor//"#3c3535"
            border.width: 1

            ColumnLayout {
                id: userInfoContent
                width: parent.width
                spacing: 10

                Text {
                    id: userInfoText
                    text: qsTr("User form:")
                    font.pixelSize: 16
                    font.weight: Font.Normal
                    font.family: montserratFont.name
                    color: Colors.currentTheme.textColor //"#dfdfdf"
                    Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                    Layout.rightMargin: 10
                    Layout.leftMargin: 10
                    Layout.topMargin: 10
                }

                CustomRowSingleTextInput {
                    id: userNameText

                    label: qsTr("Full name:")
                    input: licenseModel.usrName;
                }

                CustomRowDblTextInput {
                    id: userEmailPhone

                    label1: qsTr("Email:")
                    input1: licenseModel.usrEmail;
                    inputWidth1: 150

                    label2: qsTr("Phone:")
                    input2: licenseModel.usrPhone;
                }

                CustomRowSingleTextInput {
                    id: orgName

                    label: qsTr("Company:")
                    input: licenseModel.usrCompany;
                }
            }
        }

        Rectangle {
            id: equipmentList
            height: eqContentItem.height + 10

            Layout.fillWidth: true
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
            Layout.rightMargin: 50
            Layout.leftMargin: 50
            Layout.topMargin: 10
            Layout.minimumWidth: 500 // Minimum width it can shrink to

            color: "transparent"
            radius: 10
            border.color: Colors.currentTheme.borderColor//"#3c3535"
            border.width: 1
            ColumnLayout {
                id: eqContentItem
                width: parent.width
                spacing: 5
                Repeater {
                    id: eqList
                    model: [
                        { name: "Equipment 1", region: "Region 1", address: "Address 1" },
                        { name: "Equipment 2", region: "Region 2", address: "Address 2" },
                        { name: "Equipment 3", region: "Region 3", address: "Address 3" }
                    ]

                    delegate:
                        ColumnLayout {
                        id: equipmentInfoContent
                        width: parent.width
                        spacing: 10

                        Text {
                            color: Colors.currentTheme.helpTextColor
                            text: qsTr("Equipment ") + (index+1) + ": "
                            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                            Layout.leftMargin: 10
                            Layout.topMargin: 10

                            font.pixelSize: 13
                            font.family: montserratFont.name
                        }

                        CustomRowDblTextInput {
                            id: equipNameRegion

                            label1: qsTr("Name:")
                            input1: modelData.name
                            inputWidth1: 150

                            label2: qsTr("Region:")
                            input2: modelData.region
                        }

                        CustomRowSingleTextInput {
                            id: equipAddress

                            label: qsTr("Address:")
                            input: modelData.address
                        }
                    }

                }
            }
        }

        Rectangle {
            id: appKey
            height: appKeyContent.height + 30

            Layout.fillWidth: true
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
            Layout.rightMargin: 50
            Layout.leftMargin: 50
            Layout.topMargin: 10
            color: "transparent"
            radius: 10
            border.color: Colors.currentTheme.borderColor//"#3c3535"
            border.width: 1

            ColumnLayout {
                id: appKeyContent
                width: parent.width

                Text {
                    id: appKeyText
                    text: qsTr("Application key:")
                    font.pixelSize: 16
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
                    font.pixelSize: 16
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
                        fontSize: 16
                        enabled: !licenseModel.isValid

                        onClicked: {
                            licenseModel.generateKey();
                        }
                    }

                    CustomButton {
                        id: sendButton
                        Layout.minimumWidth: 150
                        Layout.minimumHeight: 50
                        buttonText: "Send"
                        showIcon: true
                        iconPath: "./images/icons/email.svg"
                        fontSize: 16
                        onClicked: {
                            licenseModel.sendEmail();
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
            border.width: 1

            ColumnLayout {
                id: licenseContent
                width: parent.width
                Text {
                    id: licenseText
                    text: qsTr("License number:")
                    font.pixelSize: 16
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
                    font.pixelSize: 16
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
                    Layout.leftMargin: 10

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

                    Layout.leftMargin: 10
                    Layout.topMargin: 20

                    font.pixelSize: 13
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
                        buttonText: "Add license"
                        showIcon: true
                        iconPath: "./images/icons/folder-plus.svg" //(Colors.currentTheme == Colors.dark) ? "./images/icons/keyboard_backspace_white_right.svg" : "./images/icons/keyboard_backspace_black_right.svg"
                        fontSize: 16
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
                        fontSize: 16
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
