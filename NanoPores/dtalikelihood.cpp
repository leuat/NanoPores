#include "dtalikelihood.h"
#include "GeometryLibrary/geometrylibrary.h"
#include "GeometryLibrary/noise.h"
#include <QDebug>


DTALikelihood::DTALikelihood()
{

}

void DTALikelihood::calculateStatistics(QVector<QVector3D> &points, LGraph& graph)
{
    DistanceToAtom da(20);
    if (points.size()==0)
        return;
    da.compute(points, 30.0);
    QVector<QPointF> hist = da.histogram(100);
    graph.fromQVector(hist);
    graph.normalizeArea();

}

void DTALikelihood::CalculateModel(Parameters *p)
{
    GeometryLibrary gl;
    gl.initialize(GeometryLibrary::GeometryModel::Regular, Noise::Simplex, p);
    m_modelParticles.clear();
//    qDebug() << "Original particles count: " << m_originalParticles->size();
    m_originalParticles->BoundingBox();
    for (Particle* pos : m_originalParticles->getParticles()) {
        QVector3D p = pos->getPos()/m_originalParticles->getBoundsSize()*10;
            if (!gl.isInVoid(p))
                m_modelParticles.append(pos->getPos());
       }

  //  qDebug() << "Model particles count: " << m_modelParticles.size();

    calculateStatistics(m_modelParticles,m_model);
}
