import QtQuick 2.5
import QtQuick.Window 2.2
import SimVis 1.0
import MySimulator 1.0
import WorkerData 1.0
import QtQuick.Controls 1.4


Window {
    visible: true
    width: 1600
    height: 1024
    MySimulator {
        id: simulator
        data: WorkerData{
            threshold: sliderThreshold.value
            value1: sliderScale.value
            value2: sliderOctaves.value
            persistence: sliderPersistence.value
            slice: sliderSlice.value
            sharpness: sliderSharpness.value
            abs: sliderAbs.value

        }
    }



    Visualizer {
        anchors.fill: parent
        simulator: simulator
        camera: camera
        backgroundColor: "black"

        /*        SkyBox {
            id: skybox
            camera: camera
            texture: ":/cubemap.jpg"
        }
*/
        TrackballNavigator {
            id: navigator
            anchors.fill: parent
            camera: camera
        }

        Spheres {
            id: spheres
            visible: true
            color: "white"
            scale: sliderSize.value


            Light {
                id: light
                ambientColor: Qt.rgba(1, 0.3, 0.05, 1);
                specularColor: "white"
                diffuseColor: Qt.rgba(0.2, 0.5, 1.0, 1);
                ambientIntensity: 0.05
                diffuseIntensity: 1
                specularIntensity: 0.02
                shininess: 50.0
                attenuation: 0.2
                position: camera.position
            }

            /*            SkyBoxReflection {
                id: reflection
                skybox: skybox
                reflectivity: 0.2
            }
*/
            SimplexBump {
                id: simplexBump
                enabled: true
                intensity: 0.1
                scale: 1.0
            }
        }
    }

    Rectangle {
        width: 400
        height: 400
        color: Qt.rgba(0.7, 0.3, 0.2, 0.4)
        radius: 10

        Row {
            x: 5
            y: 5
            Label {
                text: "Threshold:"
                color: "white"
            }

            Slider {
                id: sliderThreshold
                value: 0.5
                minimumValue: -1
                maximumValue: 1
                width: parent.parent.width
            }
        }
        Row {
            x: 5
            y: 40
            Label {
                text: "Scale:"
                color: "white"
            }

            Slider {
                id: sliderScale
                value: 1
                minimumValue: 0
                maximumValue: 4
                width: parent.parent.width
            }
        }
        Row {
            x: 5
            y: 75
            Label {
                text: "Octaves:"
                color: "white"
            }

            Slider {
                id: sliderOctaves
                value: 1
                minimumValue: 1
                maximumValue: 8
                width: parent.parent.width
            }
        }
        Row {
            x: 5
            y: 110
            Label {
                text: "Size:"
                color: "white"
            }

            Slider {
                id: sliderSize
                value: 0.05
                minimumValue: 0
                maximumValue: 0.5
                width: parent.parent.width
            }
        }
        Row {
            x: 5
            y: 145
            Label {
                text: "Slice:"
                color: "white"
            }

            Slider {
                id: sliderSlice
                value: 1
                minimumValue: 0
                maximumValue: 1
                width: parent.parent.width
            }
        }
        Row {
            x: 5
            y: 180
            Label {
                text: "Persistence:"
                color: "white"
            }

            Slider {
                id: sliderPersistence
                value: 1
                minimumValue: 0
                maximumValue: 2
                width: parent.parent.width
            }
        }
        Row {
            x: 5
            y: 215
            Label {
                text: "Sharpness:"
                color: "white"
            }

            Slider {
                id: sliderSharpness
                value: 1
                minimumValue: 0
                maximumValue: 4
                width: parent.parent.width
            }
        }
        Row {
            x: 5
            y: 240
            Label {
                text: "Abs:"
                color: "white"
            }

            Slider {
                id: sliderAbs
                value: 1
                minimumValue: 0
                maximumValue: 1
                width: parent.parent.width
            }
        }
    }

    Camera {
        id: camera

    }


}

