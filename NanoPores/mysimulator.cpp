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


void MyWorker::ConstrainParticles() {
    if (workerData==nullptr)
        return;

    m_positions.clear();

    CSimplex noise((int)workerData->value2(), 1, workerData->persistence(), 1253);

    float LARGE = 1E10;

    QVector3D boundsMin(LARGE, LARGE, LARGE);
    QVector3D boundsMax(-LARGE, -LARGE, -LARGE);
    for (QVector3D p: m_orgPositions) {
        for (int i=0;i<3;i++) {
            boundsMin[i] = min(boundsMin[i],p[i]);
            boundsMax[i] = max(boundsMax[i],p[i]);
        }
    }

    for (QVector3D pos : m_orgPositions) {

        QVector3D p = pos*workerData->value1();
        if (!(pos.x()<boundsMin[0]*workerData->slice() || pos.x()>boundsMax[0]*workerData->slice()))
        {

            float val = noise.Get(p.x(), p.y(),p.z());
            val = pow(val, workerData->sharpness());

            if (workerData->abs()>0.5)
                val = fabs(val);

            if (val>workerData->threshold())
                m_positions.append(pos);
       }
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
    IO::open("/Users/nicolaasgroeneboom/work/code/fys/NanoPores/data/chinchin.xyz", m_orgPositions);


    ConstrainParticles();
}


void MyWorker::synchronizeSimulator(Simulator *simulator)
{
    MySimulator *mySimulator = qobject_cast<MySimulator*>(simulator);
    if(mySimulator) {
        workerData = mySimulator->data();
     //   qDebug() << workerData->value1();
    }
}

void MyWorker::work()
{
    if (workerData!=nullptr) {
        ConstrainParticles();

    }
}
