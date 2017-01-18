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
    id: windowRoot
    visible: true
    width: 1600
    height: 1024

    property real splitWindow : 0.6
    property string lastBulkState
    property string lastPorousState

    RegularNoiseModel {
        id: regularNoiseModel
    }

    MultiFractalModel {
        id: multiFractalModel
    }

    MKIIModel {
        id: mkiiModel
    }

    MySimulator {
        id: simulator1
        data: WorkerData {
            id: data1
            workerName: "Left"
            enableCutting: false
            lblInfo: lblinfo1.text
        }
    }

    MySimulator {
        id: simulator2
        data: WorkerData{
            id: data2
            workerName: "Right"
            slice: sliderSlice.value
            sliceTranslate: sliderTranslate.value
            enableCutting: true
            dataSource: dataSource1
            dataSource2: dataSource2
            dataSource3: dataSource3
            lblInfo: lblinfo2.text
            //            model: multiFractalModel
            model: regularNoiseModel
//            model: mkiiModel
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
                ambientColor: Qt.rgba(1, 1.0, 1.0, 1);
                specularColor: "white"
                diffuseColor: Qt.rgba(0.4, 0.5, 1.0, 1);
                ambientIntensity: 0.2
                diffuseIntensity: 1
                specularIntensity: 0.8
                shininess: 50.0
                attenuation: 0.2
                position: camera1.position
            }

        }
        Column {
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
                onClicked: {
                    data1.fileToOpen = "file:///Users/anderhaf/Dropbox/uio/phd/2016/noisegeometry/states/sio2_100_systems_255katoms/system33.xyz"
                    data2.fileToOpen = "file:///Users/anderhaf/Dropbox/uio/phd/2016/noisegeometry/states/lammps/sio2_1mill.xyz"
                    data2.command = "loaddata "+data1.fileToOpen

                        data1.fileToOpen = "file:///Users/nicolaasgroeneboom/work/code/fys/NanoPores/data/sio2_porous.xyz"
                        data2.fileToOpen = "file:///Users/nicolaasgroeneboom/work/code/fys/NanoPores/data/sio2_bulk.xyz"
                        data2.command = "loaddata file:///Users/nicolaasgroeneboom/work/code/fys/NanoPores/data/sio2_porous.xyz"


//                    data1.fileToOpen = "file:///Users/nicolaasgroeneboom/work/code/fys/Nougat/scripts/models/mcmc_model.xyz"
//                    data2.fileToOpen = "file:///Users/nicolaasgroeneboom/work/code/fys/NanoPores/data/sio2_bulk.xyz"
//                    data2.command = "loaddata file:///Users/nicolaasgroeneboom/work/code/fys/Nougat/scripts/models/mcmc_model.xyz"

//                    data1.fileToOpen = "file:////Users/nicolaasgroeneboom/Dropbox/noisegeometry/states/mcmc_mkii_reproduce.xyz"
//                    data2.fileToOpen = "file:///Users/nicolaasgroeneboom/work/code/fys/NanoPores/data/sio2_bulk.xyz"
//                    data2.command = "loaddata file:///Users/nicolaasgroeneboom/Dropbox/noisegeometry/states/mcmc_mkii_reproduce.xyz"


                }
            }
            Button {
                id: btnStatistics
                text: "Model statistics"
                onClicked: {
                    data2.command = "calculate_model_statistics"
                }
            }
            Button {
                id: btnCurrentStatistics
                text: "Current statistics"
                onClicked: {
                    data2.command = "current_statistics"
                }
            }
            Button {
                id: btnPorosity
                text: "Porosity"
                onClicked: {
                    data2.command = "calculate_porosity"
                    data1.command = "calculate_porosity"
                }
            }

            Button {
                id: btnSaveStatistics
                text: "Save Statistics"
                onClicked: {
                    data2.command = "save_statistics"
                }
            }
            Button {
                id: btnOctree
                text: "Octree Measure"
                onClicked: {
                    //                data1.command = "calculate_octree_measure"
                    data2.command = "calculate_octree_measure"
                }
            }
            Button {
                id: btnFractalDim
                text: "Fractal dimension"
                onClicked: {
                    //                data1.command = "calculate_octree_measure"
                    data2.command = "calculate_fractal_dimension"
                }
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
            property alias lastBulkState: windowRoot.lastBulkState
            property alias lastPorousState: windowRoot.lastPorousState
            property alias previousParameterFile: paramLoadPrev.previousParameterFile
        }

        FileDialog {
            id: fileDialogOpenOriginal

            property string mode
            title: "Please choose a file"

            onAccepted: {
                if (mode=="mode1") {
                    data1.fileToOpen = fileDialogOpenOriginal.fileUrls.toString(); // open for visualizing
                    data2.command = "loaddata "+ fileDialogOpenOriginal.fileUrls.toString();
                }
                if (mode=="mode2") {
                    data2.fileToOpen = fileDialogOpenOriginal.fileUrls.toString(); // open for visualizing
                }
                if (mode=="mode3") {
                    data2.command = "loaddata "+fileDialogOpenOriginal.fileUrls.toString();
                }
                if (mode==="parameters") {
                    paramLoadPrev.previousParameterFile = fileDialogOpenOriginal.fileUrls.toString()
                    data2.model.parameters.load(fileDialogOpenOriginal.fileUrls.toString())
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
                    data2.model.parameters.save(fileDialogSave.fileUrls.toString())
                } else if(mode==="data") {
                    data2.fileToSave = fileDialogSave.fileUrls.toString();
                }

            }
        }
    }

    Visualizer {
        width: parent.width*0.5
        height: parent.height*splitWindow

        x: parent.width*0.5
        simulator: simulator2
        //        camera: Camera {
        //            id: camera2
        //        }
        camera: camera1
        backgroundColor: "black"

        TrackballNavigator {
            id: navigator2
            anchors.fill: parent
            // camera: camera2
            camera: camera1
        }

        Spheres {
            id: spheres2
            visible: true
            color: "white"
            scale: sliderSize.value


            Light {
                id: light2
                ambientColor: Qt.rgba(1, 1.0, 1.0, 1);
                specularColor: "white"
                diffuseColor: Qt.rgba(0.4, 0.5, 1.0, 1);
                ambientIntensity: 0.2
                diffuseIntensity: 1
                specularIntensity: 0.8
                shininess: 50.0
                attenuation: 0.2
                position: camera1.position
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
        color: "white"

        Figure {
            id: figure
            anchors.fill: parent
            lineGraphs: [linegraph1]
            LineGraph {
                id: linegraph1
                figure: figure
                dataSource: DataSource {
                    id: dataSource1
                }
            }
        }
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
            lineGraphs: [linegraph2, linegraph3]
            legend.visible: true

            LineGraph {

                id: linegraph2
                figure: figure2
                name: "Model"
                dataSource: DataSource {
                    id: dataSource2
                }
            }

            LineGraph {
                id: linegraph3
                figure: figure2
                name: "Data"
                dataSource: DataSource {
                    id: dataSource3
                }
            }
        }
    }

    ParametersGUI {
        id: paramGUI
        width: 400
        height: 390
        y: 180;
        x: parent.width*0.0
        color: Qt.rgba(0.7, 0.3, 0.2, 0.4)
        radius: 10
        textColor: "white"
        labelWidth: 100
        parameters: data2.model.parameters
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
                id: sliderTranslate
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
                Button {
                    id: paramLoadPrev
                    property string previousParameterFile
                    text: "Load previous"
                    onClicked: {
                        data2.model.parameters.load(previousParameterFile)
                    }
                }
                Button {
                    id: paramMonteCarlo
                    text: "Monte Carlo"
                    onClicked: {
                        data2.command = "likelihoodMonteCarlo"
                    }
                }

            }
        ]
    }
}

