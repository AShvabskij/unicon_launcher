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
        nodesModel.update();
    }

    ColumnLayout {
        id: title

        width: parent.width
        anchors.leftMargin: 27
        visible: false

         Label {
            id: heading2

            text: qsTr("Select node to connect")
            anchors.left: parent.left
            anchors.leftMargin: 27
            font.family: montserratFont.name
            font.pixelSize: 18
            font.weight: 600
//          font.family: "Titillium Web"
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

    QTunnel {
        id: tunnelModel
        onConnected:  {
            root.state = "connected"
            nodesModel.update();
        }
        onErrorRaised: {
            root.state = "error"
            msgError.text = errorTxt
            msgError.informativeText = informativeError
            msgError.visible = true
        }
    }

    Connections {
        target: nodesModel
        onErrorRaised: function(errorTxt, informativeError) {
            root.state = "error"
            msgError.text = errorTxt
            msgError.informativeText = informativeError
            msgError.visible = true
        }
    }

    MessageDialog {
        id: msgError
        title: "Error"
        text: ""
        informativeText: ""
        buttons: MessageDialog.Ok
        visible: false

//      onAccepted: Qt.quit()
    }

    Component.onCompleted: {
        root.state = "connecting"
        nodesModel.init();
        tunnelModel.connect();
    }

    Component.onDestruction: {
        nodesModel.clear();
    }
}

