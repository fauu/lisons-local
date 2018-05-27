import QtQuick 2.9
import QtGraphicalEffects 1.0

Rectangle {
    id: logoRect
    width: logoTextMetrics.width + 5
    height: logoTextMetrics.height - 8
    radius: 3

    LinearGradient {
        anchors.fill: parent
        start: Qt.point(0, 0)
        end: Qt.point(parent.width, parent.height)
        source: logoRect
        gradient: Gradient {
            GradientStop {
              position: 0
              color: "#ff9a8b"
            }
            GradientStop {
              position: 1
              color: "#ff6a88"
            }
        }
    }

    TextMetrics {
        id: logoTextMetrics
        font.family: "Lato"
        font.weight: Font.Bold
        font.pixelSize: 64
        font.letterSpacing: -3
        text: "Lisons!"
    }

    Text {
        id: logoText
        anchors.centerIn: parent
        anchors.verticalCenterOffset: -1
        anchors.horizontalCenterOffset: -1
        font: logoTextMetrics.font
        text: logoTextMetrics.text
        color: "#f9f9f9"
    }
}
