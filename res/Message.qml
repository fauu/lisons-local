import QtQuick 2.9
import "Style.js" as Style

Row {
    property alias textContent: text.text
    property alias indicator: indicatorLoader.sourceComponent
    spacing: 8

    Loader {
        id: indicatorLoader
    }

    Text {
        id: text
        anchors.verticalCenter: parent.verticalCenter
        textFormat: Text.RichText
        font.family: Style.font
        font.weight: Font.Bold
        color: Style.colors.primary;
        onLinkActivated: Qt.openUrlExternally(link)
    }
}
