#include "mysimulator.h"
#include <QDebug>
#include <iostream>
#include "io.h"
#include "distancetoatom.h"
#include "GeometryLibrary/models/noiseparameters.h"
#include "GeometryLibrary/models/multifractalparameters.h"

#include "GeometryLibrary/geometrylibrary.h"
#include "GeometryLibrary/noise.h"
using namespace std;

MySimulator::MySimulator()
{
    //   if (m_data!=nullptr)
    //     m_data->Allocate();
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

    if (workerData->fileToOpen() !="") {
        QUrl url = workerData->fileToOpen();
        m_particles.open(url.toLocalFile().toStdString().c_str());
        workerData->setFileToOpen("");
    }
}

void MyWorker::saveFile()
{
    if (workerData==nullptr)
        return;

    if (workerData->fileToSave() !="") {
        QUrl url = workerData->fileToSave();
        Particles newList;
        constrainParticles(nullptr, &newList);
        newList.save(url.toLocalFile().toStdString().c_str());
        workerData->setFileToSave("");
    }
}

void MyWorker::manageCommands()
{
    if (workerData->command()!="") {
        qDebug() << "command: " << workerData->command();
        QStringList cmd = workerData->command().toLower().split(" ");
        if (cmd[0]=="statistics") {
            qDebug() << "Starting analysis on parameter: " << cmd[1];
            //calculateStatistics();
            m_likelihood.setOriginalInput(&m_particles);
            if (m_dataParticles.size()==0) {
                qDebug() << "ZERO particles in data input!";
                workerData->setCommand("");
                return;
            }
            Parameters* p = workerData->noiseParameters();

            m_likelihood.bruteForce1D(10, p->getParameter(cmd[1]), p);
        }
        if (cmd[0]=="loaddata") {
            QUrl url = cmd[1];
            m_dataParticles.open(url.toLocalFile().toStdString().c_str());
            m_likelihood.setDataInput(&m_dataParticles);
            qDebug() << "File loaded suxxessfully.";
        }
        if (cmd[0] =="calculate_porosity")
            calculatePorosity();

        if (cmd[0]=="calculate_model_statistics") {
            m_likelihood.setOriginalInput(&m_particles);
            if (m_dataParticles.size()==0) {
                qDebug() << "ZERO particles in data input!";
                workerData->setCommand("");
                return;
            }

            calculateModelStatistics();
        }
    }
    workerData->setCommand("");

    if (m_likelihood.tick()) {
        workerData->dataSource()->setPoints(m_likelihood.likelihood().toQVector());
        workerData->dataSource2()->setPoints(m_likelihood.model().toQVector());
        workerData->dataSource3()->setPoints(m_likelihood.data().toQVector());
    }
    if (m_likelihood.getDone()){
        m_likelihood.setDone(false);
        workerData->dataSource2()->setPoints(m_likelihood.model().toQVector());
        qDebug() << "Min value: " << m_likelihood.getMinVal();
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

void MyWorker::calculateModelStatistics()
{
    NoiseParameters *np = workerData->noiseParameters();
    m_likelihood.modelAnalysis(15, np);
}


void MyWorker::constrainParticles(Spheres* spheres, Particles* extraList) {
    if (workerData==nullptr)
        return;

    if (spheres!=nullptr) {
        spheres->colors().clear();
        spheres->scales().clear();
        spheres->positions().clear();
    }
    if (m_particles.size()==0)
        return;

    NoiseParameters *np = workerData->noiseParameters();

    if(!np) {
        return;
    }
    m_particles.BoundingBox();

    if (workerData->enableCutting()) {
        GeometryLibrary gl;
        gl.initialize(GeometryLibrary::GeometryModel::Regular, Noise::Simplex, np);
        const int numberOfParticles = m_particles.getParticles().size();
        QVector<bool> shouldBeAdded;
        shouldBeAdded.resize(numberOfParticles);
        memset(&shouldBeAdded[0], 0, shouldBeAdded.size()*sizeof(bool));

#pragma omp parallel for num_threads(8)
        for(int i=0; i<numberOfParticles; i++) {
            Particle *pos = m_particles.getParticles()[i];
            const QVector3D realPos = pos->getPos();
            QVector3D scaledPos = realPos/m_particles.getBoundsSize()*10;
            if (!(realPos[0] + m_particles.getBoundsSize()*workerData->sharpness()<m_particles.getBoundsMin()[0]*workerData->slice() ||
                  realPos[0] + m_particles.getBoundsSize()*workerData->sharpness()>m_particles.getBoundsMax()[0]*workerData->slice()))
            {
                if (!gl.isInVoid(scaledPos)) {
                    shouldBeAdded[i] = true;
                }
            }
        }
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



MyWorker::MyWorker()
{

}


void MyWorker::synchronizeSimulator(Simulator *simulator)
{
    MySimulator *mySimulator = qobject_cast<MySimulator*>(simulator);
    if(mySimulator) {
        workerData = mySimulator->data();
        openFile();
        saveFile();
        manageCommands();
    }
}

void MyWorker::work()
{
    if (workerData!=nullptr) {
        constrainParticles(&m_spheres, nullptr);
    }
}
