import QtQuick 2.9
import QtQuick.Window 2.2

Window {
    visible: true
    title: "Lisons! Standalone"
    width: 500
    height: 300
    color: "#f9f9f9"

    Logo {
        id: logo
        y: 15
        anchors.horizontalCenter: parent.horizontalCenter
    }

    Text {
        id: text1
        x: 239
        y: 182
        text: backend.downloadManagerState
        font.pixelSize: 18
        color: "#282828"
    }

    Row {
        anchors.top: logo.bottom
        anchors.topMargin: 40
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 8

        Rectangle {
            width: iconImage.width
            height: iconImage.height
            radius: 3
            color: "#88d15e"
            Image {
                id: iconImage
                anchors.verticalCenter: parent.verticalCenter
                source: "assets/icons/check.png"
            }
        }

        Rectangle {
            width: iconImage2.width
            height: iconImage2.height
            radius: 3
            color: "#ffb300"
            Image {
                id: iconImage2
                anchors.verticalCenter: parent.verticalCenter
                source: "assets/icons/exclamation.png"
            }
        }

        Rectangle {
            width: iconImage3.width
            height: iconImage3.height
            radius: 3
            color: "#ff5a4b"
            Image {
                id: iconImage3
                anchors.verticalCenter: parent.verticalCenter
                source: "assets/icons/close.png"
            }
        }

        Text {
            anchors.verticalCenter: parent.verticalCenter
            text: "Server is running at <a href='http://localhost:8080'>http://localhost:8080</a>"
            font.family: "Lato"
            font.weight: Font.Bold
        }
    }

    Text {
        id: text2
        x: 239
        y: 222
        text: backend.serverState
        font.pixelSize: 18
        color: "#282828"
    }
}
