#ifndef DTALIKELIHOOD_H
#define DTALIKELIHOOD_H

#include "GeometryLibrary/likelihood/likelihood.h"
#include "particle.h"
#include "distancetoatom.h"
#include <QVector>
#include <QVector3D>

class DTALikelihood: public Likelihood
{
private:
    QVector<QVector3D> m_modelParticles;
    Particles* m_originalParticles;
    QVector<QVector3D> m_dataParticles;

    int m_numberOfRandomVectors = 8192;
    int m_histogramBins = 100;
    double m_cutoff = 20;

public:
    DTALikelihood();

    void setDataInput(Particles* dataParticles) {
        dataParticles->getVector3DList(m_dataParticles);
        calculateStatistics(m_dataParticles, m_data);
//        m_currentData = m_data.toQVector();

    }
    void setOriginalInput(Particles* originalParticles) {
        //modelParticles->getVector3DList(m_modelParticles);
        //calculateStatistics(m_modelParticles, m_model);
        //m_currentModel = m_model.toQVector();
        m_originalParticles = originalParticles;
    }


    void calculateStatistics(QVector<QVector3D>& particleList, LGraph& graph);

    void calculateModel(Parameters* p) override;


    QVector<QPointF> currentModel() const;
    QVector<QPointF> currentData() const;
    int voxelsPerDimension() const;
    void setVoxelsPerDimension(int voxelsPerDimension);
    int histogramBins() const;
    void setHistogramBins(int histogramBins);
    double cutoff() const;
    void setCutoff(double cutoff);
};

#endif // DTALIKELIHOOD_H
