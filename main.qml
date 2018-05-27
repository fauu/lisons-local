import QtQuick 2.9
import QtQuick.Window 2.2

Window {
    visible: true
    title: "Lisons! Standalone"
    width: 500
    height: 300
    color: "#f9f9f9"

    Logo {
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

    Text {
        id: text2
        x: 239
        y: 222
        text: backend.serverState
        font.pixelSize: 18
        color: "#282828"
    }
}
