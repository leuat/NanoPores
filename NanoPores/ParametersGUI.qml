import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1


Rectangle {
    id: propertiesGUIRoot
    property variant parameters
    property real labelWidth: 80
    property color textColor: "black"
    property list<Item> additional
    color: "red"

    MouseArea {
        anchors.fill: parent
        drag.target: parent
    }

    Column {
        anchors.margins: 10
        anchors.fill: parent
        id: column
        spacing: 5

    }

    onParametersChanged: {
        rebuild()
    }

    function rebuild() {
        if(parameters === undefined) {
            return
        }

        for(var i = column.children.length; i > 0 ; i--) {
            column.children[i-1].destroy()
        }

        var propertyList = parameters["parameters"]
        for(var propertyIndex in propertyList) {
            var aProperty = propertyList[propertyIndex]
            addSlider(aProperty)
        }
        for(var additionialIndex in propertiesGUIRoot.additional) {
            var button = propertiesGUIRoot.additional[additionialIndex]
            button.parent = column
        }
    }

    function addSlider(aProperty) {
        var component = Qt.createComponent("ParameterSlider.qml");
        if (component.status === Component.Ready) {
            var QMLObject = component.createObject(column, {});
            if(QMLObject===null) {
                console.log("Could not add slider...")
            } else {
                QMLObject.textColor = propertiesGUIRoot.textColor
                QMLObject.minimumValue = aProperty.min
                QMLObject.maximumValue = aProperty.max
                QMLObject.name = aProperty.name
                QMLObject.width = propertiesGUIRoot.width - 20
                QMLObject.labelWidth = propertiesGUIRoot.labelWidth
                QMLObject.value = aProperty.value
                QMLObject.stepSize = aProperty.stepSize
                QMLObject.valueChanged.connect(function() {
                    aProperty.value = QMLObject.value
                })
                aProperty.valueChanged.connect(function() {
                    QMLObject.value = aProperty.value
                })
            }
        } else {
            console.log("Could not add slider...")
        }
    }
}
