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

        Rectangle {
            id: infoPanel
            height: 300
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
                text: qsTr(" This tool allows you to connect to the selected device's web interface, enabling you to monitor its performance parameters and usage statistics.")
//                      + "<br><br>" + `Visit:  <a href="https://solconigel.com/">Solcon-IGEL Group</a> for more information`
                      + "<br>"
                      + `<h3>Work Modes</h3>`
                      + `<ul>`
                      + `<li><span class="mode-title">DIRECT</span> – For connecting to supported devices directly or over a local network.</li>`
                      + `<li><span class="mode-title">REMOTE</span> – For remote connection to supported devices (requires a license).</li>`
                      + `<li><span class="mode-title">DASHBOARD</span> – Displays usage statistics graphically.</li>`
                      + `</ul>`

                onLinkActivated: {
                    Qt.openUrlExternally(link);
                }

                anchors.fill: parent
                anchors.topMargin: 18
                anchors.bottomMargin: 18
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
            id: license
            height: licenseContent.height + 20
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
            Layout.rightMargin: 50
            Layout.leftMargin: 50
            Layout.topMargin: 50
            color: "transparent"
            radius: 10
            border.color: Colors.currentTheme.borderColor
            border.width: 1

            ColumnLayout {
                id: licenseContent
                width: parent.width
                spacing: 10

                RowLayout {

                    Layout.topMargin: 10
                    Layout.rightMargin: 10

                    Text {
                        id: licNumberLabel
                        text: licenseModel.isExists ? qsTr("License registered to:") : qsTr("NO LICENSE FOUND")
                        font.pixelSize: 18 - Constants.fontCoeff
                        font.weight: Font.Normal
                        font.family: montserratFont.name
                        color: Colors.currentTheme.textColor
                        Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                        Layout.leftMargin: 10
                        Layout.preferredWidth: 100 // Set a fixed width for the label to align both fields
                    }
                }

                CustomRowSingleText {
                    id: userNameText

                    label.text: qsTr("User:")
                    text.text: licenseModel.usrName;
                }

                CustomRowSingleText{
                    id: userEmail

                    label.text: qsTr("Email:")
                    text.text: licenseModel.usrEmail;
                }

                CustomRowSingleText{
                    id: userPhone

                    label.text: qsTr("Phone:")
                    text.text: licenseModel.usrPhone;
                }

                CustomRowSingleText {
                    id: orgName

                    label.text: qsTr("Company:")
                    text.text: licenseModel.usrCompany;
                }

                CustomRowSingleText {
                    id: licNumber

                    label.text: qsTr("License:")
                    text.text: licenseModel.licenseNumber
                }

                CustomButton {
                    id: sendButton
                    Layout.alignment: Qt.AlignLeft// | Qt.AlignTop
                    Layout.leftMargin: 10
                    Layout.minimumWidth: 200
                    Layout.minimumHeight: 50
                    buttonText: "Request license"
                    showIcon: true
                    iconPath: "./images/icons/email.svg"
                    fontSize: 16 - Constants.fontCoeff
                    visible: !licenseModel.isExists
                    onClicked: {
                        applicationFlow.settingsButton();
                    }
                }

            }
        }

    }
}
