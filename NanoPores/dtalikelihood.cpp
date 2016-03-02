#include "dtalikelihood.h"
#include <QDebug>
#include "GeometryLibrary/models/model.h"

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

void DTALikelihood::calculateModel(Model *model)
{
    m_modelParticles.clear();
    m_originalParticles->boundingBox();

    for (Particle* pos : m_originalParticles->getParticles()) {
        QVector3D scaledPos = pos->getPos()/m_originalParticles->getBoundsSize()*10;
        if (!model->isInVoid(scaledPos)) {
            m_modelParticles.append(pos->getPos());
        }
    }
    calculateStatistics(m_modelParticles,m_modelData);
}
