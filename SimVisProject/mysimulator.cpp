#include "mysimulator.h"
#include <QDebug>
MySimulator::MySimulator()
{

}

SimulatorWorker *MySimulator::createWorker()
{
    return new MyWorker();
}

MyWorker::MyWorker()
{
    using namespace SimVis;
    int numberOfTriangles = 100000;
    double maxDistanceFromOrigo = 100;
    // Initialize your worker here
    for(int triangleId=0; triangleId<numberOfTriangles; triangleId++) {
        // Each triangle has 3 vertices, randomly placed
        float x = maxDistanceFromOrigo*(2.0*rand()/double(RAND_MAX) - 1.0);
        float y = maxDistanceFromOrigo*(2.0*rand()/double(RAND_MAX) - 1.0);
        float z = maxDistanceFromOrigo*(2.0*rand()/double(RAND_MAX) - 1.0);
        QVector3D center = QVector3D(x,y,z);

        TriangleCollectionVBOData vertices[3];
        TriangleCollectionVBOData &p1 = vertices[0];
        TriangleCollectionVBOData &p2 = vertices[1];
        TriangleCollectionVBOData &p3 = vertices[2];
        for(int vertexId=0; vertexId<3; vertexId++) {
            // Relative displacement of vertices is between 0 and 2
            float x = 2.0*rand()/double(RAND_MAX);
            float y = 2.0*rand()/double(RAND_MAX);
            float z = 2.0*rand()/double(RAND_MAX);
            vertices[vertexId].vertex = QVector3D(x,y,z) + center;
            vertices[vertexId].color = QVector3D(0.5*x, 0.5*y, 0.5*z); // Random color based on displacement
        }

        // Set normal vectors
        QVector3D normal = QVector3D::crossProduct((p1.vertex-p3.vertex), (p1.vertex-p2.vertex)).normalized();
        p1.normal = normal;
        p2.normal = normal;
        p3.normal = normal;
        m_vertices.push_back(p1);
        m_vertices.push_back(p2);
        m_vertices.push_back(p3);
    }
}

void MyWorker::synchronizeSimulator(Simulator *simulator)
{
    MySimulator *mySimulator = qobject_cast<MySimulator*>(simulator);
    if(mySimulator) {
        // Synchronize data between QML thread and computing thread (MySimulator is on QML, MyWorker is computing thread).
        // This is for instance data from user through GUI (sliders, buttons, text fields etc)

    }
}

void MyWorker::synchronizeRenderer(Renderable *renderableObject)
{
    // Synchronize with renderables.
    TriangleCollection* triangleCollection = qobject_cast<TriangleCollection*>(renderableObject);
    if(triangleCollection) {
        // Update triangle collection renderable. Similarly if you use other renderables.
        triangleCollection->data = m_vertices;
        triangleCollection->dirty = true;
        return;
    }
}

void MyWorker::work()
{
    using namespace SimVis;
    // This is where all the work is being done. Here you can for example run your timesteps in a simulator.
    for(TriangleCollectionVBOData &vertex : m_vertices) {
        float dx = 0.05*((2.0*rand()/double(RAND_MAX)) - 1.0);
        float dy = 0.05*((2.0*rand()/double(RAND_MAX)) - 1.0);
        float dz = 0.05*((2.0*rand()/double(RAND_MAX)) - 1.0);
        vertex.vertex[0] += dx;
        vertex.vertex[1] += dy;
        vertex.vertex[2] += dz;
    }
}
