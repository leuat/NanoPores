#include "mysimulator.h"
#include <QDebug>
#include <iostream>
#include "io.h"
#include "distancetoatom.h"
#include "GeometryLibrary/noise.h"
#include "GeometryLibrary/models/octree.h"

using namespace std;

MySimulator::MySimulator()
{
    //   if (m_data!=nullptr)
    //     m_data->Allocate();
}

WorkerData *MySimulator::data() const
{
    return m_data;
}

void MySimulator::setData(WorkerData *data)
{
    if (m_data == data)
        return;

    m_data = data;
    emit dataChanged(data);
}

SimulatorWorker *MySimulator::createWorker()
{
    return new MyWorker();
}



void MyWorker::addParticleToSphere(Particle* p, Spheres *spheres, Particles* extraList) {
    QVector3D color = p->getType().color;
    if (spheres!=nullptr) {
        spheres->colors().append(QColor(color.x()*255.0, color.y()*255.0, color.z()*255.0, 1));
        spheres->positions().append( m_particles.scalePos(p,5));
        spheres->scales().append(p->getType().size);
    }
    if (extraList!=nullptr) {
        extraList->getParticles().append(p);
    }
}

void MyWorker::openFile()
{
    if (workerData==nullptr)
        return;

    if (!workerData->fileToOpen().isEmpty()) {
        QUrl url = workerData->fileToOpen();
        qDebug() << workerData->workerName() << " is opening file " << url;
        m_particles.open(url.toLocalFile().toStdString().c_str());
        workerData->setFileToOpen("");
    }
}

void MyWorker::saveFile()
{
    if (workerData==nullptr)
        return;

    if (!workerData->fileToSave().isEmpty()) {
        QUrl url = workerData->fileToSave();
        Particles newList;
        constrainParticles(nullptr, &newList);
        newList.save(url.toLocalFile().toStdString().c_str());
        workerData->setFileToSave("");
    }
}

bool MyWorker::manageCommands()
{
    if (!workerData->command().isEmpty()) {
        QStringList cmd = workerData->command().toLower().split(" ");
        QString command = cmd[0];
        if (command=="statistics") {
            m_likelihood.setOriginalInput(&m_particles);
            if (m_dataParticles.size()==0) {
                qDebug() << "ZERO particles in data input!";
                workerData->setCommand("");
                return false;
            }
            m_likelihood.bruteForce1D(20, cmd[1], workerData->model());
        } else if (command=="likelihoodmontecarlo") {
            m_likelihood.setOriginalInput(&m_particles);
            if (m_dataParticles.size()==0) {
                qDebug() << "ZERO particles in data input!";
                workerData->setCommand("");
                return false;
            }
            m_likelihood.monteCarlo(workerData->model(), 10);
        } else if (command=="loaddata") {
            QUrl url = cmd[1];
            m_dataParticles.open(url.toLocalFile().toStdString().c_str());
            m_likelihood.setDataInput(&m_dataParticles);
        } else if (command =="calculate_porosity") {
            calculatePorosity();
        } else if (command == "save_statistics") {
            saveStatistics();
        } else if (command == "calculate_octree_measure") {
            calculateOctree();
        } else if (command=="calculate_model_statistics") {
            m_likelihood.setOriginalInput(&m_particles);
            if (m_dataParticles.size()==0) {
                qDebug() << "ZERO particles in data input!";
                workerData->setCommand("");
                return false;
            }

            calculateModelStatistics();
        } else {
            qDebug() << workerData->workerName() << "Could not parse command: " << cmd[0];
        }
        workerData->setCommand("");
        return true;
    }

    return true;
}

void MyWorker::calculateOctree() {
    if (workerData==nullptr)
        return;

    for (int i=0;i<=1;i++) {

        Octree oct;
        oct.setThreshold(4);
        QVector<QVector3D> list;
        if (i==0)
            m_dataParticles.getVector3DList(list);
        if (i==1)
            m_particles.getVector3DList(list);

        oct.setPoints(list);
        oct.CalculateBoundingbox();

        oct.setMaxDepth(7);
        oct.buildTree(true);
        oct.melt();
        QVector<QPointF> measure;
        oct.calculateOctreeMeasure(measure);
        for (int i=0;i<measure.size();i++) {
            if (measure[i].y()!=0)
                measure[i].setY(log(measure[i].y()));
            qDebug() << measure[i];

        }
        if (i==0)
            workerData->dataSource()->setPoints(measure, true);
        if (i==1)
            workerData->dataSource2()->setPoints(measure, true);

      }


}


void MyWorker::calculateStatistics()
{
    if (workerData==nullptr)
        return;
    Particles newList;
    // First, copy particles to single list
    if (m_particles.size()==0)
        return;

    constrainParticles(nullptr, &newList);
    QVector<QVector3D> points;
    newList.getVector3DList(points);
    DistanceToAtom da(20);

    da.compute(points, 30.0);
    QVector<QPointF> hist = da.histogram(100);


    workerData->dataSource()->setPoints(hist, true);

    /*
    workerData->dataSource()->clear();
    for (int i=0;i<100;i++)
        workerData->dataSource()->addPoint(i, rand()%100);
*/
}

void MyWorker::saveStatistics()
{
    string dir = "";

    LGraph dataDTA = m_likelihood.calculateStatisticsDirect(m_dataParticles);
    Particles newList;
    constrainParticles(nullptr, &newList);
    LGraph modelDTA = m_likelihood.calculateStatisticsDirect(newList);

    dataDTA.SaveText(dir + "dta_data.txt");
    modelDTA.SaveText(dir +"dta_model.txt");

    m_likelihood.setOriginalInput(&m_particles);
    m_likelihood.modelAnalysis(50, workerData->model());

    while (m_likelihood.getDone()==false)
        if (m_likelihood.tick()) {
            m_likelihood.getStatistics().average().SaveText("model_average.txt");
            m_likelihood.getStatistics().sigma().SaveText("model_sigma.txt");
        }
}

void MyWorker::calculateModelStatistics()
{
    m_likelihood.modelAnalysis(15, workerData->model());
}


void MyWorker::constrainParticles(Spheres* spheres, Particles* extraList) {
    if (workerData==nullptr)
        return;

    if (spheres!=nullptr) {
        spheres->colors().clear();
        spheres->scales().clear();
        spheres->positions().clear();
    }
    if (m_particles.size()==0) {
        return;
    }

    m_particles.calculateBoundingBox();
    if (workerData->enableCutting()) {
        Model *model = workerData->model();
        if(!model) return;

        const int numberOfParticles = m_particles.getParticles().size();
        QVector<bool> shouldBeAdded;
        shouldBeAdded.resize(numberOfParticles);
        memset(&shouldBeAdded[0], 0, shouldBeAdded.size()*sizeof(bool));

        model->start();
        // #pragma omp parallel for num_threads(8)
        for(int i=0; i<numberOfParticles; i++) {
            Particle *particle = m_particles.getParticles()[i];
            const QVector3D position = particle->getPos();
            // QVector3D scaledPos = position/m_particles.getBoundsSize()*10;
            if (!(position[0] + m_particles.getBoundsSize()*workerData->sliceTranslate()<m_particles.getBoundsMin()[0]*workerData->slice() ||
                  position[0] + m_particles.getBoundsSize()*workerData->sliceTranslate()>m_particles.getBoundsMax()[0]*workerData->slice()))
            {
                if (!model->isInVoid(position)) {
                    shouldBeAdded[i] = true;
                }
            }
        }

        model->stop();

        for(int i=0; i<numberOfParticles; i++) {
            Particle *pos = m_particles.getParticles()[i];
            if(shouldBeAdded[i])  addParticleToSphere(pos, spheres,extraList);
        }
        shouldBeAdded.clear();
    }
    else { // Just copy
        for (Particle* pos : m_particles.getParticles()) {
            addParticleToSphere(pos, spheres, extraList);
        }
    }
    if (spheres != nullptr) {
        QString porosity = "Porosity: " + QString("%1").arg((float)spheres->positions().length() / (float)m_particles.size()) + ", calculated: " + QString("%1").arg(m_porosity) +"\n";
        workerData->setLblInfo(porosity + "# particles: "+ QString("%1").arg(spheres->positions().size()));
    }
}

void MyWorker::calculatePorosity() {
    Particles newList;
    constrainParticles(nullptr, &newList);
    newList.calculatePorosity();
    m_porosity = newList.getCalculatedPorosity();
}


void MyWorker::synchronizeRenderer(Renderable *renderableObject) {
    Spheres *spheres = qobject_cast<Spheres*>(renderableObject);

    if(spheres &&  (m_particles.size()!=0)) {
        spheres->setColors(m_spheres.colors());
        spheres->setPositions(m_spheres.positions());
        spheres->setScales(m_spheres.scales());

        spheres->setDirty(true);
        return;
    }

}

void MyWorker::synchronizeSimulator(Simulator *simulator)
{
    MySimulator *mySimulator = qobject_cast<MySimulator*>(simulator);
    if(mySimulator) {
        workerData = mySimulator->data();
        openFile();
        saveFile();
        bool everythingOK = manageCommands();

        if(everythingOK) {
            if (m_likelihood.tick()) {
                workerData->dataSource()->setPoints(m_likelihood.likelihood().toQVector());
                workerData->dataSource2()->setPoints(m_likelihood.modelData().toQVector());
                workerData->dataSource3()->setPoints(m_likelihood.data().toQVector());
            }
            if (m_likelihood.getDone()){
                m_likelihood.setDone(false);
                workerData->dataSource2()->setPoints(m_likelihood.modelData().toQVector());
                qDebug() << "Min value: " << m_likelihood.getMinVal();
            }
        }
    }
}

void MyWorker::work()
{
    if (workerData!=nullptr) {
        constrainParticles(&m_spheres, nullptr);
    }
}

float WorkerData::slice() const
{
    return m_slice;
}
bool WorkerData::enableCutting() const
{
    return m_enableCutting;
}
QString WorkerData::fileToOpen() const
{
    return m_fileToOpen;
}
QString WorkerData::fileToSave() const
{
    return m_fileToSave;
}
QString WorkerData::lblInfo() const
{
    return m_lblInfo;
}
QString WorkerData::command() const
{
    return m_command;
}
DataSource *WorkerData::dataSource() const
{
    return m_dataSource;
}
DataSource *WorkerData::dataSource2() const
{
    return m_dataSource2;
}
DataSource *WorkerData::dataSource3() const
{
    return m_dataSource3;
}
QString WorkerData::workerName() const
{
    return m_workerName;
}
Model *WorkerData::model() const
{
    return m_model;
}
float WorkerData::sliceTranslate() const
{
    return m_sliceTranslate;
}
void WorkerData::allocate() {
    if (initialized)
        return;
    initialized = true;
}
void WorkerData::setSlice(float slice)
{
    if (m_slice == slice)
        return;

    m_slice = slice;
    emit sliceChanged(slice);
}
void WorkerData::setEnableCutting(bool enableCutting)
{
    if (m_enableCutting == enableCutting)
        return;

    m_enableCutting = enableCutting;
    emit enableCuttingChanged(enableCutting);
}
void WorkerData::setFileToOpen(QString fileToOpen)
{
    if (m_fileToOpen == fileToOpen)
        return;

    m_fileToOpen = fileToOpen;
    emit fileToOpenChanged(fileToOpen);
}
void WorkerData::setFileToSave(QString fileToSave)
{
    if (m_fileToSave == fileToSave)
        return;

    m_fileToSave = fileToSave;
    emit fileToSaveChanged(fileToSave);
}
void WorkerData::setLblInfo(QString lblInfo)
{
    if (m_lblInfo == lblInfo)
        return;

    m_lblInfo = lblInfo;
    emit lblInfoChanged(lblInfo);
}
void WorkerData::setCommand(QString command)
{
    if (m_command == command)
        return;

    m_command = command;
    emit commandChanged(command);
}
void WorkerData::setDataSource(DataSource *dataSource)
{
    if (m_dataSource == dataSource)
        return;

    m_dataSource = dataSource;
    emit dataSourceChanged(dataSource);
}
void WorkerData::setDataSource2(DataSource *dataSource2)
{
    if (m_dataSource2 == dataSource2)
        return;

    m_dataSource2 = dataSource2;
    emit dataSource2Changed(dataSource2);
}
void WorkerData::setDataSource3(DataSource *dataSource3)
{
    if (m_dataSource3 == dataSource3)
        return;

    m_dataSource3 = dataSource3;
    emit dataSource3Changed(dataSource3);
}
void WorkerData::setWorkerName(QString workerName)
{
    if (m_workerName == workerName)
        return;

    m_workerName = workerName;
    emit workerNameChanged(workerName);
}
void WorkerData::setModel(Model *model)
{
    if (m_model == model)
        return;

    m_model = model;
    emit modelChanged(model);
}
void WorkerData::setSliceTranslate(float sliceTranslate)
{
    if (m_sliceTranslate == sliceTranslate)
        return;

    m_sliceTranslate = sliceTranslate;
    emit sliceTranslateChanged(sliceTranslate);
}
