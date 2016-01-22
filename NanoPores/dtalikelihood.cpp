#include "dtalikelihood.h"
#include "GeometryLibrary/geometrylibrary.h"
#include "GeometryLibrary/noise.h"
#include <QDebug>


int DTALikelihood::voxelsPerDimension() const
{
    return m_voxelsPerDimension;
}

void DTALikelihood::setVoxelsPerDimension(int voxelsPerDimension)
{
    m_voxelsPerDimension = voxelsPerDimension;
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

void DTALikelihood::calculateStatistics(QVector<QVector3D> &points, LGraph& graph)
{
    DistanceToAtom da(m_voxelsPerDimension); // voxes_per_dimension
    if (points.size()==0)
        return;
    da.compute(points, m_cutoff); // cutoff
    QVector<QPointF> hist = da.histogram(m_histogramBins); // bins
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
