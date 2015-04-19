import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2

ApplicationWindow {
    title: qsTr("Hello World")
    width: 640
    height: 480

    menuBar: MenuBar {
        Menu {
            title: qsTr("&File")
            MenuItem {
                text: qsTr("&Open")
                onTriggered: messageDialog.show(qsTr("Open action triggered"));
            }
            MenuItem {
                text: qsTr("E&xit")
                onTriggered: Qt.quit();
            }
        }
    }

    MainForm {
        anchors.fill: parent
        button1.onClicked: messageDialog.show(qsTr("Button 1 pressed"))
        button2.onClicked: messageDialog.show(qsTr("Button 2 pressed"))
        button3.onClicked: messageDialog.show(qsTr("Button 3 pressed"))

        RadioButton {
            id: radioButton1
            x: 454
            y: 71
            text: qsTr("Radio Button")
        }

        RadioButton {
            id: radioButton2
            x: 454
            y: 100
            text: qsTr("Radio Button")
        }

        RadioButton {
            id: radioButton3
            x: 454
            y: 134
            text: qsTr("Radio Button")
        }

        Slider {
            id: sliderHorizontal1
            x: 392
            y: 175
        }

        Slider {
            id: sliderVertical1
            x: 325
            y: 12
            orientation: Qt.Vertical
        }

        Slider {
            id: sliderVertical2
            x: 297
            y: 12
            orientation: Qt.Vertical
        }

        Slider {
            id: sliderVertical3
            x: 269
            y: 12
            orientation: Qt.Vertical
        }
    }

    MessageDialog {
        id: messageDialog
        title: qsTr("May I have your attention, please?")

        function show(caption) {
            messageDialog.text = caption;
            messageDialog.open();
        }
    }
}
