#ifndef MYSIMULATOR_H
#define MYSIMULATOR_H
#include <SimVis/Simulator>
#include <SimVis/TriangleCollection>

#include <QVector>
#include <QVector3D>

class MyWorker : public SimulatorWorker
{
    Q_OBJECT
private:
    // SimulatorWorker interface
    virtual void synchronizeSimulator(Simulator *simulator);
    virtual void synchronizeRenderer(Renderable *renderableObject);
    virtual void work();
    QVector<SimVis::TriangleCollectionVBOData> m_vertices;
public:
    MyWorker();
};

class MySimulator : public Simulator
{
    Q_OBJECT
private:

public:
    MySimulator();

    // Simulator interface
protected:
    virtual SimulatorWorker *createWorker();
};

#endif // MYSIMULATOR_H
