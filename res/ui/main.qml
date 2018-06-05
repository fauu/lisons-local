import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Layouts 1.2
import "Style.js" as Style

Window {
    readonly property int windowMargin: 20
    readonly property int layoutSpacing: 10
    readonly property int logoBottomMargin: 20
    readonly property int messageContainerHeight: 20

    visible: true
    title: "Lisons! Local"
    width: 500
    height: 2 * windowMargin
            + logo.height
            + logoBottomMargin
            + 2 * layoutSpacing
            + 2 * messageContainerHeight
	minimumWidth: width
	minimumHeight: height
	maximumWidth: minimumWidth
	maximumHeight: minimumHeight
    color: Style.colors.secondary

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: windowMargin
        spacing: layoutSpacing

        Logo {
            id: logo
            Layout.alignment: Qt.AlignHCenter
            Layout.bottomMargin: logoBottomMargin
        }

        Item {
            id: distUpdaterStateMessageContainer
            width: parent.width
            height: messageContainerHeight
            Message {
                visible: backend.distUpdaterState == 0
                indicator: loadingIndicator
                textContent: "Downloading manifest file"
            }
            Message {
                visible: backend.distUpdaterState == 1
                indicator: loadingIndicator
                textContent: "Downloading Lisons!"
            }
            Message {
                visible: backend.distUpdaterState == 2
                indicator: successIndicator
                textContent: "Lisons! is up to date"
            }
            Message {
                visible: backend.distUpdaterState == 3
                indicator: warningIndicator
                textContent: "Could not update Lisons!"
            }
            Message {
                visible: backend.distUpdaterState == 4
                indicator: failureIndicator
                textContent: "Could not download Lisons!"
            }
        }

        Item {
            id: serverStateMessageContainer
            width: parent.width
            height: messageContainerHeight
            Message {
                visible: backend.serverState == 1
                indicator: successIndicator
                textContent: "
                    <style>a:link { color: " + Style.colors.accent + "; }</style>
                    Server is running at
                    <a href='" + backend.serverAddress + "'>
                        " + backend.serverAddress + "
                    </a>"
            }
            Message {
                visible: backend.serverState == 2
                indicator: failureIndicator
                textContent: "Could not start the server"
            }
        }
    }


    Component {
        id: loadingIndicator
        Indicator {
            color: "transparent"
            icon: "loading"
            spinning: true
        }
    }

    Component {
        id: successIndicator
        Indicator {
            color: Style.colors.success
            icon: "check"
        }
    }

    Component {
        id: warningIndicator
        Indicator {
            color: Style.colors.warning
            icon: "exclamation"
        }
    }

    Component {
        id: failureIndicator
        Indicator {
            color: Style.colors.failure
            icon: "close"
        }
    }
}
