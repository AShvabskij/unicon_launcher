import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Dialogs
import qmlmodels 1.0
// import QtCore

Item {
    id: root
    state: ""
    property bool licenseValidity: licenseModel.isValid
    property int horizontMargins: 30

    FontLoader {
        id: montserratFont
        source: "./fonts/montserrat/montserrat-regular.ttf"
    }


    FileDialog {
        id: fileDialog
        title: "Select a .dat file"
//      currentFolder: StandardPaths.standardLocations(StandardPaths.DownloadLocation)[0]
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
    }

    ColumnLayout {
        id: gridLayout
        width: parent.width
        anchors.top: parent.top
        anchors.topMargin: 20

        Rectangle {
            id: infoPanel
            height: 40
            color: "Transparent" //Colors.currentTheme.boxColor
            radius: 10
            border.color: Colors.currentTheme.borderColor
            border.width: 0

            Layout.fillWidth: true
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
            Layout.rightMargin: horizontMargins
            Layout.leftMargin: horizontMargins

            Text {
                id: infoText
                color: Colors.currentTheme.textColor
                textFormat: Text.RichText
                text: qsTr(" Please fill the form to get remote access to your devices")
                onLinkActivated: {
                    Qt.openUrlExternally(link);
                }

                anchors.fill: parent
                anchors.topMargin: 10
                anchors.bottomMargin: 10
                anchors.leftMargin: 24
                anchors.rightMargin: 24
                font.pixelSize: 16 - Constants.fontCoeff
                font.weight: Font.Normal
                font.family: montserratFont.name
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignTop
                wrapMode: Text.Wrap
            }
        }

        Rectangle {
            id: userForm
            height: userFormContent.height + 20

            Layout.fillWidth: true
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
            Layout.rightMargin: horizontMargins
            Layout.leftMargin: horizontMargins
            Layout.topMargin: 10
            Layout.minimumWidth: 500 // Minimum width it can shrink to

            color: "transparent"
            radius: 10
            border.color: Colors.currentTheme.borderColor//"#3c3535"
            border.width: 1

            ColumnLayout {
                Layout.topMargin: 10
                id: userFormContent
                width: parent.width
                spacing: 10

                // Text {
                //     id: userFormText
                //     color: Colors.currentTheme.helpTextColor
                //     text: qsTr("Please fill the form and click to request license")

                //     Layout.leftMargin: 10
                //     Layout.topMargin: 10

                //     font.pixelSize: 15
                //     font.family: montserratFont.name
                // }

                CustomRowSingleTextInput {
                    id: userNameText
                    Layout.topMargin: 10
                    label.text: qsTr("User name:")
                    input.text: licenseModel.usrName;
                    input.onTextChanged: {
                        licenseModel.usrName = input.text;
                    }
                }

                CustomRowDblTextInput {
                    id: userEmailPhone

                    label1: qsTr("Email:")
                    input1.text: licenseModel.usrEmail;
                    inputWidth1: 150
                    input1.onTextChanged: {
                        licenseModel.usrEmail = input1.text;
                    }

                    label2: qsTr("Phone:")
                    input2.text: licenseModel.usrPhone;
                    input2.onTextChanged: {
                        licenseModel.usrPhone = input2.text;
                    }

                }

                CustomRowSingleTextInput {
                    id: orgName

                    label.text: qsTr("Company:")
                    input.text: licenseModel.usrCompany;
                    input.onTextChanged: {
                        licenseModel.usrCompany = input.text
                    }
                }

                Repeater {
                    id: eqList
                    // model: [
                    //     { name: "Equipment 1", region: "Region 1", address: "Address 1" },
                    //     { name: "Equipment 2", region: "Region 2", address: "Address 2" },
                    //     { name: "Equipment 3", region: "Region 3", address: "Address 3" }
                    // ]

                    model: licenseModel.equipmentList
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

                            font.pixelSize: 13 - Constants.fontCoeff
                            font.family: montserratFont.name
                        }

                        CustomRowDblTextInput {
                            id: equipNameRegion

                            label1: qsTr("Name:")
                            input1.text: model.name
                            inputWidth1: 150
                            input1.onEditingFinished: {
                                var equipObj = {
                                    "name": input1.text,
                                };

                                licenseModel.equipmentList.update(index, equipObj)
                            }

                            label2: qsTr("Region:")
                            input2.text: model.region
                            input2.onEditingFinished: {
                                var equipObj = {
                                    "name": model.name,
                                    "region": input2.text,
                                    "address": model.address
                                };

                                licenseModel.equipmentList.update(index, equipObj)

                            }
                        }

                        CustomRowSingleTextInput {
                            id: equipAddress

                            label.text: qsTr("Address:")
                            input.text: model.address
                            input.onEditingFinished: {
                                var equipObj = {
                                    "name": model.name,
                                    "region": model.region,
                                    "address": input.text
                                };

                                licenseModel.equipmentList.update(index, equipObj)
                            }
                        }
                    }

                }
            }
        }

        Rectangle {
            id: license
            height: licenseContent.height + 20
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
            Layout.rightMargin: horizontMargins
            Layout.leftMargin: horizontMargins
            Layout.topMargin: 10
            color: "transparent"
            radius: 10
            border.color: Colors.currentTheme.borderColor
            border.width: 1

            ColumnLayout {
                id: licenseContent
                width: parent.width

                CustomRowSingleTextInput {
                    id: appKey
                    Layout.topMargin: 10

                    label.text: qsTr("App key:")
                    input.text: licenseModel.appKey;
                    input.color: Colors.currentTheme.helpTextColor
                    input.readOnly: true
                }

                CustomRowSingleTextInput {
                    id: licNumber

                    Layout.topMargin: 10
                    label.text: qsTr("License :")
                    input.text: licenseModel.licenseNumber;
                }

                Text {
                    id: statusText
                    color: !licenseModel.isExists ? Colors.currentTheme.helpTextColor : (licenseValidity ? "#227102" : "#CC0000")
                    text: {
                        function genStatusText() {
                            if (!licenseModel.isExists) {
                                return qsTr("Please request the license and add the license file received from the support team");
                            }

                            return licenseValidity ? qsTr("License activated successfully") : qsTr("Invalid license");
                        }

                        return genStatusText();  // Call the function
                    }

                    Layout.leftMargin: 10
                    Layout.topMargin: 6

                    font.pixelSize: 13 - Constants.fontCoeff
                    font.family:  montserratFont.name
//                  visible: licenseModel.isExists
                }

                RowLayout {
                    // anchors.top: helpLicenseText.bottom
                    // anchors.left: helpLicenseText.left
                    Layout.leftMargin: 10
                    Layout.topMargin: 10
                    spacing: 20
                    Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                    Layout.minimumHeight: 40

                    CustomButton {
                        id: sendButton
                        Layout.alignment: Qt.AlignCenter// | Qt.AlignTop
                        Layout.minimumWidth: 200
                        Layout.minimumHeight: 50
                        buttonText: "Request license"
                        showIcon: true
                        iconPath: "./images/icons/email.svg"
                        fontSize: 16 - Constants.fontCoeff
                        onClicked: {
                            licenseModel.sendRequest();
                        }
                    }
                    CustomButton {
                        anchors.topMargin: 50
                        id: licenseButton
                        Layout.minimumWidth: 200
                        Layout.minimumHeight: 50
                        buttonText: "Add license"
                        showIcon: true
                        iconPath: "./images/icons/folder-plus.svg" //(Colors.currentTheme == Colors.dark) ? "./images/icons/keyboard_backspace_white_right.svg" : "./images/icons/keyboard_backspace_black_right.svg"
                        fontSize: 16 - Constants.fontCoeff
                        onClicked: {
                            fileDialog.open();
                        }
                    }

                    CustomButton {
                        anchors.topMargin: 50
                        id: licenseButtonRemove
                        Layout.minimumWidth: 200
                        Layout.minimumHeight: 50
                        buttonText: "Delete license"
                        showIcon: false
                        fontSize: 16 - Constants.fontCoeff
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
