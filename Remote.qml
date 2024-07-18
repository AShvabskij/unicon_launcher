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
//  required property var nodesList

    //  property alias model: repeater.model
    signal sendNodeClicked(string ipAddress, string name)

    Column {
        id: title

        width: parent.width

        topPadding: 34

        leftPadding: 27
        rightPadding: 27

        bottomPadding: 13

/*
        Label {
            id: heading

            text: qsTr("Avalable nodes for the license")
            font.pixelSize: 12
            font.weight: 600
            font.family: "Titillium Web"
            color:  Colors.currentTheme.textColor
            elide: Text.ElideRight
        }
*/
        Label {
            id: heading2

            text: qsTr("Select node to connect")
//          anchors.horizontalCenter: parent.horizontalCenter

            font.pixelSize: 24
            font.weight: 600
            font.family: "Titillium Web"
            color:  Colors.currentTheme.textColor
            elide: Text.ElideRight
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
            columns:  scrollView.contentWidth / (scrollView.delegatePreferredWidth  + gridLayout.columnSpacing) // scrollView.contentWidth < (scrollView.delegatePreferredWidth * 2  + gridLayout.columnSpacing) ? 1 : 2

            Repeater {
                id: repeater
                model: nodeListModel

                NodeItem {
                    id: nodeItem

                    Layout.preferredHeight: scrollView.delegatePreferredHeight
                    Layout.preferredWidth: scrollView.delegatePreferredWidth
                    Layout.alignment: Qt.AlignHCenter

                    onClicked: {
                        sendNodeClicked(ipAddress, fullName)
                    }
                }

            }
        }
    }

    QTunnel {
        id: tunnelModel
        onConnected:  {
            root.state = "connected"
            nodeListModel.update();
        }
        onErrorRaised: {
            root.state = "error"
            msgError.text = error
            msgError.informativeText = informativeError
            msgError.visible = true
        }
    }

    NodeListModel {
        id: nodeListModel
        onErrorRaised: {
            root.state = "error"
            msgError.text = error
            msgError.informativeText = informativeError
            msgError.visible = true
        }
    }

    MessageDialog {
        id: msgError
        text: ""
        informativeText: ""
        buttons: MessageDialog.Ok
        visible: false

//      onAccepted: Qt.quit()
    }

    Component.onCompleted: {
        root.state = "connecting"
        nodeListModel.init();
        tunnelModel.connect();
    }

}

