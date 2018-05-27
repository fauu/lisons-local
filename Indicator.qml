import QtQuick 2.9

Rectangle {
    property string icon
    property alias spinning: imageAnimation.running
    width: image.width
    height: image.height
    radius: 3
    Image {
        id: image
        anchors.verticalCenter: parent.verticalCenter
        source: "assets/icons/" + icon + ".png"
        RotationAnimator on rotation {
            id: imageAnimation
            running: false
            from: 0;
            to: 360;
            duration: 1000
            loops: Animation.Infinite
        }
    }
}
