import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects
import QtQuick.Dialogs
import QtQuick.Window 2.2

import qmlmodels 1.0

Item {
    id: root
    state: ""
    required property var nodesModel

    //  property alias model: repeater.model
    signal sendNodeClicked(string ipAddress, string name)
    FontLoader {
        id: montserratFont
        source: "./fonts/montserrat/montserrat-regular.ttf"
    }

    function onRefreshPage() {
        licenseModel.parseLicense();
        if (licenseModel.isExists) {
            root.state = "connecting"
            nodesModel.init();
            tunnelModel.connect();
        } else {
            applicationFlow.backButton();
        }
    }

    ColumnLayout {
        id: title

        width: parent.width
        anchors.leftMargin: 27
        anchors.topMargin: 20

        visible: true

        RowLayout{
            Layout.alignment: Qt.AlignRight | Qt.AlignTop
            Layout.rightMargin: 27
            Layout.topMargin: 10

            Image {
                id: icon
                visible: licenseModel.usrName != ""
                source: "./images/icons/user-circle.svg"
                sourceSize.width: heading2.height
                sourceSize.height: heading2.height

                Layout.alignment: Qt.AlignRight
                Layout.rightMargin: 10
            }

            Label {
                id: heading2

                text: licenseModel.usrName //qsTr("Select node to connect")
                font.family: montserratFont.name
                font.pixelSize: 18
                font.weight: 600
                color:  Colors.currentTheme.textColor
                elide: Text.ElideRight
            }
        }
    }

    ScrollView {
        id: scrollView
        anchors.top: title.bottom
        anchors.topMargin: 20
        leftPadding: 27
        rightPadding: 20

        height: parent.height - title.height
        width: parent.width

        clip: true
        contentWidth: availableWidth

        property int delegatePreferredHeight: 220
        property int delegatePreferredWidth: 320

        GridLayout {
            id: gridLayout

            columnSpacing: 24
            rowSpacing: 24
            flow: GridLayout.LeftToRight
            columns:  scrollView.contentWidth / (scrollView.delegatePreferredWidth  + gridLayout.columnSpacing)

            Repeater {
                id: repeater
                model: nodesModel

                NodeItem {
                    id: nodeItem

                    Layout.preferredHeight: scrollView.delegatePreferredHeight
                    Layout.preferredWidth: scrollView.delegatePreferredWidth
                    Layout.alignment: Qt.AlignHCenter

                    onClicked: {
                        if (activated) {
                            sendNodeClicked(ipAddress, fullName)
                        }
                    }
                }

            }
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
        Component.onDestruction: {
            root.state = ""
        }
    }

    QTunnel {
        id: tunnelModel
        onConnected:  {
            root.state = "connected"
            nodesModel.update();
        }
        onErrorRaised: function(errorTxt, informativeError) {
            root.state = "error"
            msgError.text = errorTxt
            msgError.informativeText = informativeError
            msgError.visible = true
        }
    }

    Connections {
        target: nodesModel
        function onErrorRaised(errorTxt, informativeError) {
            root.state = "warning"
            msgError.text = errorTxt
            msgError.informativeText = informativeError
            msgError.visible = true
        }
    }

    WarningBox {
        id: msgError
        onClosed: {
            if (!licenseModel.isExists) {
                applicationFlow.settingsButton()
            }
        }
    }

    Component.onCompleted: {

        licenseModel.parseLicense();
        if (licenseModel.isExists) {
            root.state = "connecting"
            nodesModel.init();
            tunnelModel.connect();
        } else {
            root.state = "Info"
            msgError.text = "NO LICENSE FOUND"
            msgError.informativeText = "Please request license before starting remote support"
            msgError.visible = true
        }

    }

    Component.onDestruction: {
        nodesModel.clear();
    }
}

