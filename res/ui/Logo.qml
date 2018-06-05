import QtQuick 2.9
import QtGraphicalEffects 1.0
import "Style.js" as Style

Rectangle {
	readonly property int widthOffset: {
	  switch (Qt.platform.os) {
	    case "windows": return 20;
		default: return 5;
   	  }
	}
	readonly property int heightOffset: -8

    id: logoRect
    width: logoTextMetrics.width + widthOffset
    height: logoTextMetrics.height + heightOffset
    radius: Style.sizes.borderRadius

    LinearGradient {
        anchors.fill: parent
        start: Qt.point(0, 0)
        end: Qt.point(parent.width, parent.height)
        source: logoRect
        gradient: Gradient {
            GradientStop {
              position: 0
              color: Style.colors.accentAlt
            }
            GradientStop {
              position: 1
              color: Style.colors.accent
            }
        }
    }

    TextMetrics {
        id: logoTextMetrics
        font.family: Style.font
        font.weight: Font.Bold
        font.pixelSize: 64
        font.letterSpacing: -3
        text: "Lisons!"
    }

    Text {
        id: logoText
        anchors.centerIn: parent
        anchors.verticalCenterOffset: -1
        font: logoTextMetrics.font
        text: logoTextMetrics.text
        color: Style.colors.secondary
    }
}
