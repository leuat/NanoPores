import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1


Rectangle {
    id: propertiesGUIRoot
    property variant parameters
    property real labelWidth: 80
    property color textColor: "black"
    property list<Item> additional
    width: column.width
    height: column.height
    color: "white"

    MouseArea {
        anchors.fill: parent
        drag.target: parent
    }

    Column {
        id: column
        anchors.margins: 10
        spacing: 5
        width: 400
        height: children.length*25

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
            if(aProperty.string !== "") {
                addTextfield(aProperty)
            } else {
                if(aProperty.min === 0 && aProperty.max === 1 && aProperty.stepSize === 1.0) {
                    addSwitch(aProperty)
                } else {
                    addSlider(aProperty)
                }

            }
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
                // QMLObject.width = propertiesGUIRoot.width - 20
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

    function addTextfield(aProperty) {
        var component = Qt.createComponent("ParameterTextfield.qml");
        if (component.status === Component.Ready) {
            var QMLObject = component.createObject(column, {});
            if(QMLObject===null) {
                console.log("Could not add textfield...")
            } else {
                QMLObject.textColor = propertiesGUIRoot.textColor
                QMLObject.labelWidth = propertiesGUIRoot.labelWidth
                QMLObject.name = aProperty.name
                QMLObject.string = aProperty.string

                QMLObject.stringChanged.connect(function() {
                    aProperty.string = QMLObject.string
                })
                aProperty.stringChanged.connect(function() {
                    QMLObject.string = aProperty.string
                })
            }
        } else {
            console.log("Could not add textfield...")
        }
    }

    function addSwitch(aProperty) {
        var component = Qt.createComponent("ParameterSwitch.qml");
        if (component.status === Component.Ready) {
            var QMLObject = component.createObject(column, {});
            if(QMLObject===null) {
                console.log("Could not add switch...")
            } else {
                QMLObject.textColor = propertiesGUIRoot.textColor
                QMLObject.labelWidth = propertiesGUIRoot.labelWidth
                QMLObject.name = aProperty.name
                QMLObject.checked = aProperty.value === 1.0 ? true : false

                QMLObject.checkedChanged.connect(function() {
                    aProperty.value = QMLObject.checked ? 1.0 : 0.0
                })
                aProperty.valueChanged.connect(function() {
                    QMLObject.checked = aProperty.value === 1.0 ? true : false
                })
            }
        } else {
            console.log("Could not add aProperty.value = QMLObject.checked ? 1.0 : 0.0...")
        }
    }
}
