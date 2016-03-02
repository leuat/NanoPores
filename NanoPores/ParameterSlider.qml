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
    property alias stepSize: mySlider.stepSize
    property real labelWidth: 80
    property bool buttonVisible: true
    spacing: 5
    height: 20
    width: parent.width

    Label {
        text: name+' '
        color: rootItem.textColor
        width: rootItem.labelWidth
    }
    Label {
        text: '' + parseFloat(Math.round(value * 1000) / 1000).toFixed(3) +' : '
        color: rootItem.textColor
        width: rootItem.labelWidth/2
    }

    Slider {
        id: mySlider
        width: rootItem.width - 2.0*labelWidth - rootItem.spacing*3
    }

    Button {
        width: labelWidth/2;
        visible: rootItem.buttonVisible
        text: "F"
        onClicked: {
            data2.command = "statistics " + name
        }

    }
}
