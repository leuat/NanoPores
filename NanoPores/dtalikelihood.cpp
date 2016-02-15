#include "dtalikelihood.h"
#include "GeometryLibrary/geometrylibrary.h"
#include "GeometryLibrary/noise.h"
#include <QDebug>


int DTALikelihood::voxelsPerDimension() const
{
    return m_numberOfRandomVectors;
}

void DTALikelihood::setVoxelsPerDimension(int voxelsPerDimension)
{
    m_numberOfRandomVectors = voxelsPerDimension;
}

int DTALikelihood::histogramBins() const
{
    return m_histogramBins;
}

void DTALikelihood::setHistogramBins(int histogramBins)
{
    m_histogramBins = histogramBins;
}

double DTALikelihood::cutoff() const
{
    return m_cutoff;
}

void DTALikelihood::setCutoff(double cutoff)
{
    m_cutoff = cutoff;
}

DTALikelihood::DTALikelihood()
{

}

LGraph DTALikelihood::calculateStatisticsDirect(Particles &particles)
{
    QVector<QVector3D> qp;
    particles.getVector3DList(qp);
    LGraph graph;
    calculateStatistics(qp, graph);
    return graph;

}

void DTALikelihood::calculateStatistics(QVector<QVector3D> &points, LGraph& graph)
{
    DistanceToAtom da(m_numberOfRandomVectors); // voxes_per_dimension
    if (points.size()==0)
        return;
    da.compute(points, m_cutoff); // cutoff
    QVector<QPointF> hist = da.histogram(m_histogramBins); // bins
    graph.fromQVector(hist);
    graph.normalizeArea();

}

void DTALikelihood::calculateModel(Parameters *p)
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
