#include "mysimulator.h"
#include <QDebug>
#include "io.h"
#include "CSimplex.h"
#include "distancetoatom.h"

MySimulator::MySimulator()
{
 //   if (m_data!=nullptr)
   //     m_data->Allocate();
}

SimulatorWorker *MySimulator::createWorker()
{
    return new MyWorker();
}



void MyWorker::AddParticleToSphere(Particle* p, Spheres *spheres, Particles* extraList) {
    QVector3D qc = p->getType().color;
    if (spheres!=nullptr) {
        spheres->colors().append(QColor(qc.x()*255.0, qc.y()*255.0, qc.z()*255.0, 1));
        spheres->positions().append( m_particles.ScalePos(p,5));
        spheres->scales().append(p->getType().size);
    }
    if (extraList!=nullptr)
        extraList->getParticles().append(p);
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
        if (workerData->command().toLower()=="statistics") {
            calculateStatistics();
        }
    }
    workerData->setCommand("");
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


    workerData->dataSource()->SetSource(hist);
    qDebug() << workerData->dataSource()->m_points << endl;

/*
    workerData->dataSource()->clear();
    for (int i=0;i<100;i++)
        workerData->dataSource()->addPoint(i, rand()%100);
*/
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

    CSimplex noise((int)workerData->value2(), 1, workerData->persistence(), 1253);
    m_particles.BoundingBox();
    if (workerData->enableCutting())
    for (Particle* pos : m_particles.getParticles()) {

        QVector3D p = pos->getPos()*workerData->value1()/m_particles.getBoundsSize()*10;
        if (!(pos->getPos()[0] + m_particles.getBoundsSize()*workerData->sharpness()<m_particles.getBoundsMin()[0]*workerData->slice() ||
              pos->getPos()[0] + m_particles.getBoundsSize()*workerData->sharpness()>m_particles.getBoundsMax()[0]*workerData->slice()))
        {

            float val = noise.Get(p.x(), p.y(),p.z());
            //val = pow(val, workerData->sharpness());

            if (workerData->abs()>0.5)
                val = fabs(val);
            if (workerData->invert()<0.5) {
                if (val>workerData->threshold())
                    AddParticleToSphere(pos, spheres,extraList);
               }
            else {
                if (val<workerData->threshold())
                    AddParticleToSphere(pos, spheres,extraList);
            }
       }
    }
    else { // Just copy
        for (Particle* pos : m_particles.getParticles())
            AddParticleToSphere(pos, spheres, extraList);
    }
    if (spheres != nullptr)
    workerData->setLblInfo("# particles: "+ QString("%1").arg(spheres->positions().size()));


 //   workerData->dataSource()->addPoint(workerData->dataSource()->size(), rand()%100);
 //   qDebug() << workerData->dataSource()->size();
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
//        workerData->Allocate();
        constrainParticles(&m_spheres, nullptr);
    }
}
