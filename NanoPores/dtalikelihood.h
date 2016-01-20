#ifndef DTALIKELIHOOD_H
#define DTALIKELIHOOD_H

#include <likelihood/likelihood.h>
#include <particle.h>
#include <distancetoatom.h>
#include <QVector>
#include <QVector3D>

class DTALikelihood: public Likelihood
{
private:
    QVector<QPointF> m_currentModel;
    QVector<QPointF> m_currentData;
    QVector<QVector3D> m_modelParticles;
    Particles* m_originalParticles;
    QVector<QVector3D> m_dataParticles;

public:
    DTALikelihood();

    void setDataInput(Particles* dataParticles) {
        dataParticles->getVector3DList(m_dataParticles);
        calculateStatistics(m_dataParticles, m_data);
        m_currentData = m_data.toQVector();

    }
    void setOriginalInput(Particles* originalParticles) {
        //modelParticles->getVector3DList(m_modelParticles);
        //calculateStatistics(m_modelParticles, m_model);
        //m_currentModel = m_model.toQVector();
        m_originalParticles = originalParticles;
    }


    void calculateStatistics(QVector<QVector3D>& particleList, LGraph& graph);

    void CalculateModel(Parameters* p) override;


    QVector<QPointF> currentModel() const;
    QVector<QPointF> currentData() const;
};

#endif // DTALIKELIHOOD_H
