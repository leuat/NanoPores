import QtQuick 2.5
import QtQuick.Window 2.0
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
import SimVis 1.0
import MySimulator 1.0
import WorkerData 1.0
import Qt.labs.settings 1.0
import GeometryLibrary 1.0
import QtCharts 2.0
import DataSource 1.0

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
        }
    }

    MySimulator {
        id: simulator2
        data: WorkerData{
            id: data2
            slice: sliderSlice.value
            sharpness: sliderSharpness.value
            enableCutting: true

            dataSource: dataSource1
            dataSource2: dataSource2
            dataSource3: dataSource3

            lblInfo: lblinfo2.text

            noiseParameters: NoiseParameters {
                id: noiseParameters
           }
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
            id: btnDirty
            text: "You dirty boy"
            y: 30
            onClicked: {
                data1.fileToOpen = "file:///Users/nicolaasgroeneboom/work/code/fys/NanoPores/data/sio2_porous.xyz"
                data2.fileToOpen = "file:///Users/nicolaasgroeneboom/work/code/fys/NanoPores/data/sio2_bulk.xyz"
                data2.command = "loaddata file:///Users/nicolaasgroeneboom/work/code/fys/NanoPores/data/sio2_porous.xyz"
            }
        }

        Label {
            id: lblinfo1
            x: 5
            y: parent.height-30
            color:  Qt.rgba(1,1,1,1)
            text: data1.lblInfo
        }

        Settings {
            property alias lastOpenedFolderOriginal: fileDialogOpenOriginal.folder
            property alias lastOpenedFolderSave: fileDialogSave.folder
        }

        FileDialog {
            id: fileDialogOpenOriginal

            property string mode
            title: "Please choose a file"

            onAccepted: {
                if (mode=="mode1") {
                    data1.fileToOpen = fileDialogOpenOriginal.fileUrls.toString();
                    data2.command = "loaddata "+ fileDialogOpenOriginal.fileUrls.toString();
                }
                if (mode=="mode2") {
                    data2.fileToOpen = fileDialogOpenOriginal.fileUrls.toString();
                }
                if (mode=="mode3") {
                    data2.command = "loaddata "+fileDialogOpenOriginal.fileUrls.toString();
                }
                if (mode==="parameters") {
                    noiseParameters.load(fileDialogOpenOriginal.fileUrls.toString())
                }
            }
        }

        FileDialog {
            id: fileDialogSave
            selectExisting : false
            property string mode
            title: "Please choose a location to save"

            onAccepted: {
                if(mode==="parameters") {
                    noiseParameters.save(fileDialogSave.fileUrls.toString())
                } else if(mode==="data") {
                    data2.fileToSave = fileDialogSave.fileUrls.toString();
                }

            }
        }
    }

    Rectangle {
        width: parent.width*0.5
        height: parent.height*(1-splitWindow)
        y: parent.height*splitWindow
        color: "white"
        Figure {
            id: figure
            anchors.fill: parent
            LineGraph {
                dataSource: DataSource {
                    id: dataSource1
                }
            }
        }
    }

//       Figure {
//           id: figure
//           width: parent.width*0.5
//           height: parent.height*(1-splitWindow)
//           y: parent.height*splitWindow
//           fitData: true
//           fitExact: true
//           xLabel: "d [Å] "
//           yLabel: "P(d)"
//           title: "Scale"
//           LineGraph {
//               id: graph1
//               dataSource: LineGraphDataSource {
//                   id: dataSource1
//               }
//               width: 2
//               // style: Qt.DotLine
//           }
//       }


    Visualizer {
        width: parent.width*0.5
        height: parent.height*splitWindow

        x: parent.width*0.5
        simulator: simulator2
        camera: Camera {
            id: camera2
        }
        backgroundColor: "black"

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
        y: 30
        id: btnOpen3
        text: "Open data"
        onClicked: {
            fileDialogOpenOriginal.mode = "mode3"
            fileDialogOpenOriginal.open()
        }
    }
    Button {
        x: parent.width*0.5
        y: 60
        id: btnSave
        text: "Save"
        onClicked: {
            fileDialogSave.mode = "data"
            fileDialogSave.open()
        }
    }

    ParametersGUI {
        id: paramGUI
        width: 400
        height: 390
        y: 120;
        x: parent.width*0.0
        color: Qt.rgba(0.7, 0.3, 0.2, 0.4)
        radius: 10
        textColor: "white"
        labelWidth: 100
        parameters: noiseParameters
        additional: [
            ParameterSlider {
                id: sliderSize
                name: "Size"
                minimumValue: 0.05
                maximumValue: 1
                value: 0.05
                stepSize: 0.1
                labelWidth: paramGUI.labelWidth
                textColor: "white"
                buttonVisible: false
            },
            ParameterSlider {
                id: sliderSlice
                name: "Slice"
                minimumValue: 0.0
                maximumValue: 1
                value: 1.0
                stepSize: 0.05
                labelWidth: paramGUI.labelWidth
                textColor: "white"
                buttonVisible: false
            },
            ParameterSlider {
                id: sliderSharpness
                name: "Translate"
                minimumValue: -1
                maximumValue: 1
                value: 0.0
                stepSize: 0.05
                labelWidth: paramGUI.labelWidth
                textColor: "white"
                buttonVisible: false
            },
            Row {
                Button {
                    id: paramLoad
                    text: "Load"
                    onClicked: {
                        fileDialogOpenOriginal.mode = "parameters"
                        fileDialogOpenOriginal.open()
                    }
                }
                Button {
                    id: paramSave
                    text: "Save"
                    onClicked: {
                        fileDialogSave.mode = "parameters"
                        fileDialogSave.open()
                    }
                }
            }
        ]


    }

    Label {
        id: lblinfo2
        x: parent.width*0.5 + 5
        y: parent.height - 30
        color: Qt.rgba(1,1,1,1)
        text: data2.lblInfo
    }

    Rectangle {
        width: parent.width*0.5
        height: parent.height*(1-splitWindow)
        y: parent.height*splitWindow
        x: parent.width*0.5
        color: "white"

        Figure {
            id: figure2
            anchors.fill: parent
            LineGraph {
                dataSource: DataSource {
                    id: dataSource2
                }
            }

            LineGraph {
                dataSource: DataSource {
                    id: dataSource3
                }
            }
        }
    }

//    Figure {
//        id: figure2
//        //anchors.fill: parent
//        //color: "red"
//        width: parent.width*0.5
//        height: parent.height*(1-splitWindow)
//        y: parent.height*splitWindow
//        x: parent.width*0.5
//        fitData: true
//        fitExact: true
//        xLabel: "d [Å]"
//        yLabel: "P(d)"
//        title: ""
//        LineGraph {
//            id: graph2
//            dataSource: LineGraphDataSource {
//                id: dataSource2
//            }
//            width: 2
//            // style: Qt.DotLine
//        }
//        LineGraph {
//            id: graph3
//            dataSource: LineGraphDataSource {
//                id: dataSource3
//            }
//            width: 2
//            color: Qt.rgba(1,0,0,1);
//            // style: Qt.DotLine
//        }
//    }
}

