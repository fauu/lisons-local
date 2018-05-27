import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Layouts 1.2
//    anchors.horizontalCenter: parent.horizontalCenter

Window {
    readonly property int windowMargins: 15
    readonly property int layoutSpacing: 10
    readonly property int logoBottomMargin: 20
    readonly property int messageContainerHeight: 20

    visible: true
    title: "Lisons! Local"
    width: 500
    height: 2 * windowMargins
            + logo.height
            + logoBottomMargin
            + 2 * layoutSpacing
            + 2 * messageContainerHeight
    color: "#f9f9f9"

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: windowMargins
        spacing: layoutSpacing

        Logo {
            id: logo
            anchors.horizontalCenter: parent.horizontalCenter
            Layout.bottomMargin: logoBottomMargin
        }

        Item {
            id: downloadManagerStateMessageContainer
            width: parent.width
            height: messageContainerHeight
            Message {
                visible: backend.downloadManagerState == 0
                indicator: loadingIndicator
                textContent: "Downloading manifest file"
            }
            Message {
                visible: backend.downloadManagerState == 1
                indicator: loadingIndicator
                textContent: "Downloading Lisons"
            }
            Message {
                visible: backend.downloadManagerState == 2
                indicator: successIndicator
                textContent: "Lisons is up to date"
            }
            Message {
                visible: backend.downloadManagerState == 3
                indicator: warningIndicator
                textContent: "Could not update Lisons – you may be running an outdated version"
            }
            Message {
                visible: backend.downloadManagerState == 4
                indicator: failureIndicator
                textContent: "Could not download Lisons"
            }
        }

        Item {
            id: serverStateMessageContainer
            width: parent.width
            height: messageContainerHeight
            Message {
                visible: backend.serverState == 1
                indicator: successIndicator
                textContent: "<style>a:link { color: #ff6a88; }</style>Server is running at <a href='http://localhost:8080'>http://localhost:8080</a>"
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
            color: "#8cc06e"
            icon: "check"
        }
    }

    Component {
        id: warningIndicator
        Indicator {
            color: "#eab73e"
            icon: "exclamation"
        }
    }

    Component {
        id: failureIndicator
        Indicator {
            color: "#df746a"
            icon: "close"
        }
    }
}
