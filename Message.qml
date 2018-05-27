import QtQuick 2.9

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
        font.family: "Lato"
        font.weight: Font.Bold
        onLinkActivated: Qt.openUrlExternally(link)
    }
}
