import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
Row {
    id: rootItem
    property string name
    property color textColor: "black"
    property alias value: mySlider.value
    property alias minimumValue: mySlider.minimumValue
    property alias maximumValue: mySlider.maximumValue
    property real labelWidth: 80
    spacing: 5
    width: 200

    Label {
        text: name+': '
        color: rootItem.textColor
        width: rootItem.labelWidth
    }
    Slider {
        id: mySlider
        width: rootItem.width - labelWidth - rootItem.spacing
    }
}
