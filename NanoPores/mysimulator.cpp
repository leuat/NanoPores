#include "mysimulator.h"
#include <QDebug>
#include "io.h"
#include "CSimplex.h"

MySimulator::MySimulator()
{

}

SimulatorWorker *MySimulator::createWorker()
{
    return new MyWorker();
}


void MyWorker::AddParticleToSphere(Particle* p, Spheres *spheres) {
    QVector3D qc = p->getType().color;
    spheres->colors().append(QColor(qc.x()*255.0, qc.y()*255.0, qc.z()*255.0, 1));
    spheres->positions().append( m_particles.ScalePos(p,5));
    spheres->scales().append(p->getType().size);
}

void MyWorker::ConstrainParticles(Spheres* spheres) {
    if (workerData==nullptr)
        return;

    spheres->colors().clear();
    spheres->scales().clear();
    spheres->positions().clear();

    CSimplex noise((int)workerData->value2(), 1, workerData->persistence(), 1253);
    m_particles.BoundingBox();
    for (Particle* pos : m_particles.getParticles()) {

        QVector3D p = pos->getPos()*workerData->value1();
        if (!(pos->getPos()[0] + m_particles.getBoundsSize()*workerData->sharpness()<m_particles.getBoundsMin()[0]*workerData->slice() ||
              pos->getPos()[0] + m_particles.getBoundsSize()*workerData->sharpness()>m_particles.getBoundsMax()[0]*workerData->slice()))
        {

            float val = noise.Get(p.x(), p.y(),p.z());
            //val = pow(val, workerData->sharpness());

            if (workerData->abs()>0.5)
                val = fabs(val);
            if (workerData->invert()<0.5) {
                if (val>workerData->threshold())
                    AddParticleToSphere(pos, spheres);
               }
            else {
                if (val<workerData->threshold())
                    AddParticleToSphere(pos, spheres);
            }
       }
    }

}

void MyWorker::synchronizeRenderer(Renderable *renderableObject) {
    Spheres *spheres = qobject_cast<Spheres*>(renderableObject);

    if(spheres) {
        spheres->setColors(m_spheres.colors());
        spheres->setPositions(m_spheres.positions());
        spheres->setScales(m_spheres.scales());

        spheres->setDirty(true);
        return;
    }

}


MyWorker::MyWorker()
{
/*    int N = 1000000;
    double box_size = 10;
    for (int i=0;i<N;i++) {
        QVector3D p;
        for (int j=0;j<3;j++)
            p[j] = box_size*((rand()/(double)RAND_MAX)- 0.5);

        m_positions.push_back(p);
    }


    IO::save("/Users/nicolaasgroeneboom/work/code/fys/NanoPores/data/chinchin.xyz", m_positions);
*/
//    m_particles.open("/Users/nicolaasgroeneboom/work/code/fys/NanoPores/data/chinchin.xyz");
    m_particles.open("/Users/nicolaasgroeneboom/work/code/fys/NanoPores/data/sio2_porous.xyz");

}


void MyWorker::synchronizeSimulator(Simulator *simulator)
{
    MySimulator *mySimulator = qobject_cast<MySimulator*>(simulator);
    if(mySimulator) {
        workerData = mySimulator->data();
    }
}

void MyWorker::work()
{
    if (workerData!=nullptr) {
        ConstrainParticles(&m_spheres);
    }
}
