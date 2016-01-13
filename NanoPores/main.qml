import QtQuick 2.5
import QtQuick.Window 2.2
import SimVis 1.0
import MySimulator 1.0
import WorkerData 1.0
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.0
import Qt.labs.settings 1.0
import QMLPlot 1.0



Window {
    visible: true
    width: 1600
    height: 1024

    property real splitWindow : 0.6


    MySimulator {
        id: simulator1
        data: WorkerData {
            id: data1
            enableCutting: false
            lblInfo: lblinfo1.text
            dataSource: LineGraphDataSource {
                id: datasource1
            }

        }
    }

    MySimulator {
        id: simulator2
        data: WorkerData{
            id: data2
            threshold: sliderThreshold.value
            value1: sliderScale.value
            value2: sliderOctaves.value
            persistence: sliderPersistence.value
            slice: sliderSlice.value
            sharpness: sliderSharpness.value
            abs: sliderAbs.value
            invert: sliderInvert.value
            enableCutting: true
            dataSource: LineGraphDataSource {
                id: datasource2
            }
            lblInfo: lblinfo2.text


        }
    }




    Visualizer {
  //      anchors.fill: parent
        width: parent.width*0.5
        height: parent.height*splitWindow;
        simulator: simulator1
        camera: Camera {
            id: camera1
        }

        backgroundColor: "black"

        TrackballNavigator {
            id: navigator1
            anchors.fill: parent
            camera: camera1
        }

        Spheres {
            id: spheres1
            visible: true
            color: "white"
            scale: sliderSize.value


            Light {
                id: light1
                ambientColor: Qt.rgba(1, 0.7, 0.5, 1);
                specularColor: "white"
                diffuseColor: Qt.rgba(0.2, 0.5, 1.0, 1);
                ambientIntensity: 0.2
                diffuseIntensity: 1
                specularIntensity: 0.8
                shininess: 50.0
                attenuation: 0.2
                position: camera1.position
            }

        }

        Button {
            id: btnOpen
            text: "Open"
            onClicked: {
                fileDialogOpenOriginal.mode = "mode1"
                fileDialogOpenOriginal.open()
            }
        }
        Button {
            id: btnCalculateStatistics
            text: "Calculate statistics"
            y: 30
            onClicked: {
                data1.command = "statistics"
                data2.command = "statistics"
            }
        }

        Label {
            id: lblinfo1
            x: 5
            y: parent.height-30
            color:  Qt.rgba(1,1,1,1)
            text: data1.lblInfo
        }

        FileDialog {
            id: fileDialogOpenOriginal

            property string mode
            title: "Please choose a file"
            //folder: shortcuts.home
            /*Settings {
                property alias url: fileDialogOpenOriginal.folder
            }
*/
            onAccepted: {
                if (mode=="mode1")
                    data1.fileToOpen = fileDialogOpenOriginal.fileUrls.toString();
                if (mode=="mode2")
                    data2.fileToOpen = fileDialogOpenOriginal.fileUrls.toString();
            }
            onRejected: {
            }
            Component.onCompleted: visible = false
        }

        FileDialog {
            id: fileDialogSave
            selectExisting : false
             property string mode
            title: "Please choose a location to save"
            //folder: shortcuts.home
            /*Settings {
                property alias url: fileDialogOpenOriginal.folder
            }
*/
            onAccepted: {
                data2.fileToSave = fileDialogSave.fileUrls.toString();
            }
            onRejected: {
            }
            Component.onCompleted: visible = false
        }

    }

       Figure {
           id: figure
           //anchors.fill: parent
           //color: "red"
           width: parent.width*0.5
           height: parent.height*(1-splitWindow)
           y: parent.height*splitWindow
           xMin: 0
           xMax: 100
           yMin: 0
           yMax: 1000000
           autoBounds: true
           xLabel: "t [s] "
           yLabel: "T [K]"
           title: "Scale"
           LineGraph {
               id: graph
               dataSource: datasource1
               width: 2
               // style: Qt.DotLine
           }
       }


    Visualizer {
        width: parent.width*0.5
        height: parent.height*splitWindow

        x: parent.width*0.5
        simulator: simulator2
        camera: Camera {
            id: camera2
        }
        backgroundColor: "black"

        /*        SkyBox {
            id: skybox
            camera: camera
            texture: ":/cubemap.jpg"
        }
*/
        TrackballNavigator {
            id: navigator2
            anchors.fill: parent
            camera: camera2
        }

        Spheres {
            id: spheres2
            visible: true
            color: "white"
            scale: sliderSize.value


            Light {
                id: light2
                ambientColor: Qt.rgba(1, 0.7, 0.5, 1);
                specularColor: "white"
                diffuseColor: Qt.rgba(0.2, 0.5, 1.0, 1);
                ambientIntensity: 0.02
                diffuseIntensity: 1
                specularIntensity: 0.8
                shininess: 50.0
                attenuation: 0.2
                position: camera2.position
            }

            /*            SkyBoxReflection {
                id: reflection
                skybox: skybox
                reflectivity: 0.2
            }
*/
            SimplexBump {
                id: simplexBump
                enabled: false
                intensity: 0.1
                scale: 1.0
            }
        }
    }
    Button {
        x: parent.width*0.5
        y: 0
        id: btnOpen2
        text: "Open"
        onClicked: {
            fileDialogOpenOriginal.mode = "mode2"
            fileDialogOpenOriginal.open()
        }
    }
    Button {
        x: parent.width*0.5
        y: 25
        id: btnSave
        text: "Save"
        onClicked: {
            fileDialogSave.open()
        }
    }

    Rectangle {
        width: 200
        height: 400
        y: 50;
        x: parent.width*0.5
        color: Qt.rgba(0.7, 0.3, 0.2, 0.4)
        radius: 10

        Row {
            x: 5
            y: 5
            width: parent.width

            Label {
                text: "Threshold:"
                color: "white"
            }

            Slider {
                id: sliderThreshold
                value: 0.5
                minimumValue: -1
                maximumValue: 1
            }
        }
        Row {
            x: 5
            y: 40
            width: parent.width
            Label {
                text: "Scale:"
                color: "white"
            }

            Slider {
                id: sliderScale
                value: 1
                minimumValue: 0
                maximumValue: 4
            }
        }
        Row {
            x: 5
            y: 75
            width: parent.width
            Label {
                text: "Octaves:"
                color: "white"
            }

            Slider {
                id: sliderOctaves
                value: 1
                minimumValue: 1
                maximumValue: 8
            }
        }
        Row {
            x: 5
            y: 110
            width: parent.width
            Label {
                text: "Size:"
                color: "white"
            }

            Slider {
                id: sliderSize
                value: 0.05
                minimumValue: 0
                maximumValue: 0.5
            }
        }
        Row {
            x: 5
            y: 145
            width: parent.width
            Label {
                text: "Slice:"
                color: "white"
            }

            Slider {
                id: sliderSlice
                value: 1
                minimumValue: 0
                maximumValue: 1
            }
        }
        Row {
            x: 5
            y: 180
            width: parent.width
            Label {
                text: "Persistence:"
                color: "white"
            }

            Slider {
                id: sliderPersistence
                value: 1
                minimumValue: 0
                maximumValue: 2
            }
        }
        Row {
            x: 5
            y: 215
            width: parent.width
            Label {
                text: "Translate:"
                color: "white"
            }

            Slider {
                id: sliderSharpness
                value: 0
                minimumValue: -1
                maximumValue: 1
            }
        }
        Row {
            x: 5
            y: 240
            width: parent.width
            Label {
                text: "Abs:"
                color: "white"
            }

            Slider {
                id: sliderAbs
                value: 1
                minimumValue: 0
                maximumValue: 1
            }
        }
        Row {
            x: 5
            y: 275
            width: parent.width
            Label {
                text: "Invert:"
                color: "white"
            }

            Slider {
                id: sliderInvert
                value: 0
                minimumValue: 0
                maximumValue: 1
            }
        }

    }
    Label {
        id: lblinfo2
        x: parent.width*0.5 + 5
        y: parent.height - 30
        color: Qt.rgba(1,1,1,1)
        text: data2.lblInfo
    }

    Figure {
        id: figure2
        //anchors.fill: parent
        //color: "red"
        width: parent.width*0.5
        height: parent.height*(1-splitWindow)
        y: parent.height*splitWindow
        x: parent.width*0.5
        xMin: 0
        xMax: 100
        yMin: 0
        yMax: 100000
        autoBounds: true
        xLabel: "t [s] "
        yLabel: "T [K]"
        title: "Scale 2"
        LineGraph {
            id: graph2
            dataSource: datasource2
            width: 2
            // style: Qt.DotLine
        }
    }




}

