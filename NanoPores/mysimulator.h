#ifndef MYSIMULATOR_H
#define MYSIMULATOR_H
#include <SimVis/Simulator>
#include <SimVis/Spheres>
#include <SimVis/QuickWorker>

#include <QVector>
#include <QVector3D>
#include <QMap>


class WorkerData : public QObject {
    Q_OBJECT

    Q_PROPERTY(float value1 READ value1 WRITE setValue1 NOTIFY value1Changed)
    Q_PROPERTY(float value2 READ value2 WRITE setValue2 NOTIFY value2Changed)
    Q_PROPERTY(float threshold READ threshold WRITE setTreshold NOTIFY thresholdChanged)
    Q_PROPERTY(float slice READ slice WRITE setSlice NOTIFY sliceChanged)
    Q_PROPERTY(float persistence READ persistence WRITE setPersistence NOTIFY persistenceChanged)
    Q_PROPERTY(float sharpness READ sharpness WRITE setSharpness NOTIFY sharpnessChanged)
    Q_PROPERTY(float abs READ abs WRITE setAbs NOTIFY absChanged)


    float m_value1;

    float m_value2;

    float m_threshold;

    float m_slice;

    float m_persistence;

    float m_sharpness;

    float m_abs;

public:
    float value1() const
    {
        return m_value1;
    }
    float value2() const
    {
        return m_value2;
    }

    float threshold() const
    {
        return m_threshold;
    }

    float slice() const
    {
        return m_slice;
    }

    float persistence() const
    {
        return m_persistence;
    }

    float sharpness() const
    {
        return m_sharpness;
    }

    float abs() const
    {
        return m_abs;
    }

public slots:
    void setValue1(float value1)
    {
        if (m_value1 == value1)
            return;

        m_value1 = value1;
        emit value1Changed(value1);
    }
    void setValue2(float value2)
    {
        if (m_value2 == value2)
            return;

        m_value2 = value2;
        emit value2Changed(value2);
    }

    void setTreshold(float threshold)
    {
        if (m_threshold == threshold)
            return;

        m_threshold = threshold;
        emit thresholdChanged(threshold);
    }

    void setSlice(float slice)
    {
        if (m_slice == slice)
            return;

        m_slice = slice;
        emit sliceChanged(slice);
    }

    void setPersistence(float persistence)
    {
        if (m_persistence == persistence)
            return;

        m_persistence = persistence;
        emit persistenceChanged(persistence);
    }

    void setSharpness(float sharpness)
    {
        if (m_sharpness == sharpness)
            return;

        m_sharpness = sharpness;
        emit sharpnessChanged(sharpness);
    }

    void setAbs(float abs)
    {
        if (m_abs == abs)
            return;

        m_abs = abs;
        emit absChanged(abs);
    }

signals:
    void value1Changed(float value1);
    void value2Changed(float value2);
    void thresholdChanged(float threshold);
    void sliceChanged(float slice);
    void persistenceChanged(float persistence);
    void sharpnessChanged(float sharpness);
    void absChanged(float abs);
};


class MyWorker : public QuickWorker
{
    Q_OBJECT


private:
    WorkerData* workerData;
    QVector<QVector3D> m_orgPositions;
    void ConstrainParticles();


    // SimulatorWorker interface
    virtual void synchronizeSimulator(Simulator *simulator);
    virtual void work();

public:
    MyWorker();
};

class MySimulator : public Simulator
{
    Q_OBJECT

    Q_PROPERTY(WorkerData* data READ data WRITE setData NOTIFY dataChanged)
    WorkerData* m_data = nullptr;

public:
    MySimulator();

    // Simulator interface


    WorkerData* data() const
    {
        return m_data;
    }

public slots:


    void setData(WorkerData* data)
    {
        if (m_data == data)
            return;

        m_data = data;
        emit dataChanged(data);
    }

signals:
    void dataChanged(WorkerData* data);

protected:
    virtual SimulatorWorker *createWorker();
};

#endif // MYSIMULATOR_H
