#ifndef PARTICLE_H
#define PARTICLE_H
#include <QVector3D>
#include <QMap>
#include <QVector>

class ParticleType {
public:
    float size;
    QVector3D color;
    QString name;

    ParticleType() {

    }

    ParticleType(QString _name, float _size, QVector3D _color) {
        size = _size;
        color = _color;
        name = _name;
    }
};


class ParticleTypes {
    QMap<QString, ParticleType> map;
public:
    static ParticleTypes& getInstance() {
        static ParticleTypes instance;
        return instance;
    }
    ParticleTypes(ParticleTypes const&) = delete;
    void operator=(ParticleTypes const&)  = delete;

    ParticleTypes() {
        map.insert("Ar", ParticleType("Ar",1, QVector3D(1,1,0.2)));
        map.insert("Si", ParticleType("Si", 1, QVector3D(1,1,0.3)));
        map.insert("O", ParticleType("O", 0.5, QVector3D(1,0.3,0.2)));
        map.insert("1", ParticleType("Si", 1, QVector3D(0.8,1,0.3)));
        map.insert("2", ParticleType("O", 0.5, QVector3D(1,0.4,0.2)));
    }

    QMap<QString, ParticleType> getMap() const;
};



class Particle
{
private:
    QVector3D pos;
    ParticleType type;

public:

    Particle();
    QVector3D getPos() const;
    void setPos(const QVector3D &value);
    ParticleType getType() const;
    void setType(const ParticleType &value);
};

class Particles {
    QVector<Particle *> particles;
    QVector3D boundsMin, boundsMax;
    float boundsSize;

public:
    QVector<Particle *>& getParticles();

    void save(const char *filename);
    void open(const char *filename);
    void copyFrom(Particles& plist);

    ~Particles() {
        //clear();
    }

    void clear() {
        for (Particle* p : particles)
            delete p;
        particles.clear();
    }



    int size() {
        return particles.size();
    }

    Particle* operator[](const int i) {
        return particles[i];
    }

    void clearPointersOnly() {
        particles.clear();
    }

    void BoundingBox( ) {
        float LARGE = 1E20;

        boundsMin = QVector3D(LARGE, LARGE, LARGE);
        boundsMax = QVector3D(-LARGE, -LARGE, -LARGE);
        for (Particle* p: particles) {
            for (int i=0;i<3;i++) {
                boundsMin[i] = std::min(boundsMin[i],p->getPos()[i]);
                boundsMax[i] = std::max(boundsMax[i],p->getPos()[i]);
            }
        }
        boundsSize = (boundsMax - boundsMin).length();
    }
    QVector3D ScalePos(Particle* p, float scale) {
        return (p->getPos() - (boundsMax+boundsMin)*0.5)/boundsSize*scale;
    }

    QVector3D getBoundsMin() const;
    QVector3D getBoundsMax() const;



    float getBoundsSize() const;
};

class NBHList {
private:
    QVector<Particles> list;

    Particle* findNBH(Particle* org, Particles& lst, double ha);

public:
    Particles& operator[](const int i) {
        return list[i];
    }

    int size() {
        return list.size();
    }

    void Create(Particles& plist);


};


#endif // PARTICLE_H
