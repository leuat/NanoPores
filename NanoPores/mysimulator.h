#ifndef MYSIMULATOR_H
#define MYSIMULATOR_H
#include <SimVis/Simulator>
#include <SimVis/Spheres>
#include <SimVis/QuickWorker>

#include <QVector>
#include <QVector3D>
#include <QMap>
#include "particle.h"
#include "QMLPlot/linegraph.h"
#include "dtalikelihood.h"
#include "GeometryLibrary/models/noiseparameters.h"

class WorkerData : public QObject {
    Q_OBJECT

    Q_PROPERTY(float value1 READ value1 WRITE setValue1 NOTIFY value1Changed)
    Q_PROPERTY(float value2 READ value2 WRITE setValue2 NOTIFY value2Changed)
    Q_PROPERTY(float threshold READ threshold WRITE setTreshold NOTIFY thresholdChanged)
    Q_PROPERTY(float slice READ slice WRITE setSlice NOTIFY sliceChanged)
    Q_PROPERTY(float persistence READ persistence WRITE setPersistence NOTIFY persistenceChanged)
    Q_PROPERTY(float sharpness READ sharpness WRITE setSharpness NOTIFY sharpnessChanged)
    Q_PROPERTY(float abs READ abs WRITE setAbs NOTIFY absChanged)
    Q_PROPERTY(float invert READ invert WRITE setInvert NOTIFY invertChanged)
    Q_PROPERTY(float skewScale READ skewScale WRITE setSkewScale NOTIFY skewScaleChanged)
    Q_PROPERTY(float skewAmplitude READ skewAmplitude WRITE setSkewAmplitude NOTIFY skewAmplitudeChanged)
    Q_PROPERTY(bool enableCutting READ enableCutting WRITE setEnableCutting NOTIFY enableCuttingChanged)
    Q_PROPERTY(QString fileToOpen READ fileToOpen WRITE setFileToOpen NOTIFY fileToOpenChanged)
    Q_PROPERTY(QString fileToSave READ fileToSave WRITE setFileToSave NOTIFY fileToSaveChanged)
    Q_PROPERTY(QString lblInfo READ lblInfo WRITE setLblInfo NOTIFY lblInfoChanged)
    Q_PROPERTY(QString command READ command WRITE setCommand NOTIFY commandChanged)

    Q_PROPERTY(LineGraphDataSource* dataSource READ dataSource WRITE setDataSource NOTIFY dataSourceChanged)
    Q_PROPERTY(LineGraphDataSource* dataSource2 READ dataSource2 WRITE setDataSource2 NOTIFY dataSource2Changed)
    Q_PROPERTY(LineGraphDataSource* dataSource3 READ dataSource3 WRITE setDataSource3 NOTIFY dataSource3Changed)
    float m_value1;

    float m_value2;

    float m_threshold;

    float m_slice;

    bool initialized = false;

    float m_persistence;

    float m_sharpness;

    float m_abs;

    float m_invert;



    bool m_enableCutting;

    QString m_fileToOpen;

    QString m_fileToSave;


    LineGraphDataSource* m_dataSource;

    QString m_lblInfo;

    QString m_command;

    float m_skewScale;

    float m_skewAmplitude;

    LineGraphDataSource* m_dataSource2;

    LineGraphDataSource* m_dataSource3;

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

    float invert() const
    {
        return m_invert;
    }

    bool enableCutting() const
    {
        return m_enableCutting;
    }

    QString fileToOpen() const
    {
        return m_fileToOpen;
    }

    QString fileToSave() const
    {
        return m_fileToSave;
    }


    LineGraphDataSource* dataSource() const
    {
        return m_dataSource;
    }

    QString lblInfo() const
    {
        return m_lblInfo;
    }

    QString command() const
    {
        return m_command;
    }

    float skewScale() const
    {
        return m_skewScale;
    }

    float skewAmplitude() const
    {
        return m_skewAmplitude;
    }

    LineGraphDataSource* dataSource2() const
    {
        return m_dataSource2;
    }

    LineGraphDataSource* dataSource3() const
    {
        return m_dataSource3;
    }

public slots:
    void Allocate() {
        if (initialized)
            return;
 //       m_dataSource = new LineGraphDataSource();
        initialized = true;
       }

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

    void setInvert(float invert)
    {
        if (m_invert == invert)
            return;

        m_invert = invert;
        emit invertChanged(invert);
    }

    void setEnableCutting(bool enableCutting)
    {
        if (m_enableCutting == enableCutting)
            return;

        m_enableCutting = enableCutting;
        emit enableCuttingChanged(enableCutting);
    }

    void setFileToOpen(QString fileToOpen)
    {
        if (m_fileToOpen == fileToOpen)
            return;

        m_fileToOpen = fileToOpen;
        emit fileToOpenChanged(fileToOpen);
    }

    void setFileToSave(QString fileToSave)
    {
        if (m_fileToSave == fileToSave)
            return;

        m_fileToSave = fileToSave;
        emit fileToSaveChanged(fileToSave);
    }


    void setDataSource(LineGraphDataSource* dataSource)
    {
        if (m_dataSource == dataSource)
            return;

        m_dataSource = dataSource;
        emit dataSourceChanged(dataSource);
    }

    void setLblInfo(QString lblInfo)
    {
        if (m_lblInfo == lblInfo)
            return;

        m_lblInfo = lblInfo;
        emit lblInfoChanged(lblInfo);
    }

    void setCommand(QString command)
    {
        if (m_command == command)
            return;

        m_command = command;
        emit commandChanged(command);
    }

    void setSkewScale(float skewScale)
    {
        if (m_skewScale == skewScale)
            return;

        m_skewScale = skewScale;
        emit skewScaleChanged(skewScale);
    }

    void setSkewAmplitude(float skewAmplitude)
    {
        if (m_skewAmplitude == skewAmplitude)
            return;

        m_skewAmplitude = skewAmplitude;
        emit skewAmplitudeChanged(skewAmplitude);
    }

    void setDataSource2(LineGraphDataSource* dataSource2)
    {
        if (m_dataSource2 == dataSource2)
            return;

        m_dataSource2 = dataSource2;
        emit dataSource2Changed(dataSource2);
    }

    void setDataSource3(LineGraphDataSource* dataSource3)
    {
        if (m_dataSource3 == dataSource3)
            return;

        m_dataSource3 = dataSource3;
        emit dataSource3Changed(dataSource3);
    }

signals:
    void value1Changed(float value1);
    void value2Changed(float value2);
    void thresholdChanged(float threshold);
    void sliceChanged(float slice);
    void persistenceChanged(float persistence);
    void sharpnessChanged(float sharpness);
    void absChanged(float abs);
    void invertChanged(float invert);
    void enableCuttingChanged(bool enableCutting);
    void fileToOpenChanged(QString fileToOpen);
    void fileToSaveChanged(QString fileToSave);
    void dataSourceChanged(LineGraphDataSource* dataSource);
    void lblInfoChanged(QString lblInfo);
    void commandChanged(QString command);
    void skewScaleChanged(float skewScale);
    void skewAmplitudeChanged(float skewAmplitude);
    void dataSource2Changed(LineGraphDataSource* dataSource2);
    void dataSource3Changed(LineGraphDataSource* dataSource3);
};


class MyWorker : public SimulatorWorker
{
    Q_OBJECT

private:
    WorkerData* workerData;
    Particles m_particles;
    Particles m_dataParticles;
    Spheres m_spheres;
    DTALikelihood m_likelihood;
    NoiseParameters* m_params = nullptr;


    void constrainParticles(Spheres* spheres, Particles* extraList);
    void AddParticleToSphere(Particle* p, Spheres *spheres, Particles* extraList);
    void openFile();
    void saveFile();
    void manageCommands();
    void calculateStatistics();
    // SimulatorWorker interface
    virtual void synchronizeSimulator(Simulator *simulator);
    virtual void work();

public:
    MyWorker();
    ~MyWorker() {
        m_particles.clear();
    }

    // SimulatorWorker interface
private:
    void synchronizeRenderer(Renderable *renderableObject);
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
