import QtQuick 2.9
import "Style.js" as Style

Rectangle {
    property string icon
    property alias spinning: imageAnimation.running

    width: image.width
    height: image.height
    radius: Style.sizes.borderRadius

    Image {
        id: image
        anchors.verticalCenter: parent.verticalCenter
        source: "/res/icons/" + icon + ".png"
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
