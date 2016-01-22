#include "particle.h"
#include <fstream>
#include <cstdlib>
#include <iostream>
#include <QFile>
#include <QTextStream>
#include <QDebug>
using namespace std;

QVector3D Particle::getPos() const
{
    return pos;

}


void Particle::setPos(const QVector3D &value)
{
    pos = value;
}

Particle::Particle()
{

}

ParticleType Particle::getType() const
{
    return type;
}

void Particle::setType(const ParticleType &value)
{
    type = value;
}


float Particles::getBoundsSize() const
{
    return boundsSize;
}

QVector<Particle *>& Particles::getParticles()
{
    return particles;
}




void Particles::open(const char *filename) {
    particles.clear();
    QFile inputFile(filename);
    int line = 0;
    if (inputFile.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFile);
       int cnt = 0;

      while (!in.atEnd())
       {
          QString line = in.readLine();
          if (cnt>=2) {
              QStringList lst = line.split(" ");
              if (lst.size()!=4)
                  continue;
              ParticleType pt = ParticleTypes::getInstance().getMap()[lst[0]];
              QVector3D pos(lst[1].toFloat(),lst[2].toFloat(),lst[3].toFloat());
              Particle* p = new Particle();
              p->setPos(pos);
//              pt.size = 1;
  //            pt.color = QVector3D(1,1,1);
              p->setType(pt);
              particles.append(p);

          }
          cnt++;

       }
       inputFile.close();
    }

    qDebug() << "HORE";
}

void Particles::copyFrom(Particles &plist)
{
    clear();
    for(Particle* p : plist.getParticles()) {
        Particle* n = new Particle();
        n->setPos(p->getPos());
        n->setType(p->getType());
        particles.append(n);
    }
}

void Particles::getVector3DList(QVector<QVector3D>& list)
{
    for(Particle* p : particles)
        list.append(p->getPos());

}

void Particles::save(const char* filename) {
    ofstream file;

    file.open(filename,std::ofstream::out);

    if(file.is_open()) {
        file << particles.count() << endl;
        file << "The is an optional comment line that can be empty." << endl;
        for(Particle* p : particles) {
            file << p->getType().name.toStdString() <<" " << p->getPos().x() << " " << p->getPos().y() << " " << p->getPos().z() << endl;
        }
        file.close();
    }
}

QVector3D Particles::getBoundsMin() const
{
    return boundsMin;
}

QVector3D Particles::getBoundsMax() const
{
    return boundsMax;
}

QMap<QString, ParticleType> ParticleTypes::getMap() const
{
    return map;
}


Particle* NBHList::findNBH(Particle *org, Particles &lst, double threshold)
{
    for(int i=0;i < lst.size();i++)
        if ((lst[i]->getPos() - org->getPos()).lengthSquared()<threshold) {
            Particle* p = lst[i];
            lst.getParticles().removeAt(i);
            return p;
        }
    return nullptr;

}

void NBHList::Create(Particles& orgList)
{
    float threshold = 0.01;
    Particles plist;
    plist.copyFrom(orgList);
    list.clear();
    Particle* p = plist[0];
    plist.getParticles().removeAt(0);





}
