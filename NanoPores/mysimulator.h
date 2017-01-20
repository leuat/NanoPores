#ifndef MYSIMULATOR_H
#define MYSIMULATOR_H
#include <SimVis/Simulator>
#include <SimVis/Spheres>
#include <SimVis/QuickWorker>

#include <QVector>
#include <QVector3D>
#include <QMap>
#include "GeometryLibrary/particle.h"
#include "QMLPlot/linegraph.h"
#include "GeometryLibrary/models/models.h"
#include "GeometryLibrary/likelihood/dtalikelihood.h"
#include "datasource.h"

class WorkerData : public QObject {
    Q_OBJECT
    Q_PROPERTY(DataSource* dataSource READ dataSource WRITE setDataSource NOTIFY dataSourceChanged)
    Q_PROPERTY(DataSource* dataSource2 READ dataSource2 WRITE setDataSource2 NOTIFY dataSource2Changed)
    Q_PROPERTY(DataSource* dataSource3 READ dataSource3 WRITE setDataSource3 NOTIFY dataSource3Changed)
    Q_PROPERTY(QString workerName READ workerName WRITE setWorkerName NOTIFY workerNameChanged)
    Q_PROPERTY(Model* model READ model WRITE setModel NOTIFY modelChanged)
    Q_PROPERTY(float slice READ slice WRITE setSlice NOTIFY sliceChanged)
    Q_PROPERTY(float sliceTranslate READ sliceTranslate WRITE setSliceTranslate NOTIFY sliceTranslateChanged)
    Q_PROPERTY(bool enableCutting READ enableCutting WRITE setEnableCutting NOTIFY enableCuttingChanged)
    Q_PROPERTY(QString fileToOpen READ fileToOpen WRITE setFileToOpen NOTIFY fileToOpenChanged)
    Q_PROPERTY(QString fileToSave READ fileToSave WRITE setFileToSave NOTIFY fileToSaveChanged)
    Q_PROPERTY(QString lblInfo READ lblInfo WRITE setLblInfo NOTIFY lblInfoChanged)
    Q_PROPERTY(QString command READ command WRITE setCommand NOTIFY commandChanged)

    Model* m_model = nullptr;
    bool initialized = false;
    float m_slice;
    bool m_enableCutting;
    QString m_fileToOpen;
    QString m_fileToSave;

    QString m_lblInfo;
    QString m_command;
    float m_skewScale;
    float m_skewAmplitude;

    DataSource* m_dataSource = nullptr;
    DataSource* m_dataSource2 = nullptr;
    DataSource* m_dataSource3 = nullptr;
    QString m_workerName; 
    float m_sliceTranslate;

public:
    float slice() const;
    bool enableCutting() const;
    QString fileToOpen() const;
    QString fileToSave() const;
    QString lblInfo() const;
    QString command() const;
    DataSource* dataSource() const;
    DataSource* dataSource2() const;
    DataSource* dataSource3() const;
    QString workerName() const;
    Model* model() const;
    float sliceTranslate() const;

public slots:
    void allocate();
    void setSlice(float slice);
    void setEnableCutting(bool enableCutting);
    void setFileToOpen(QString fileToOpen);
    void setFileToSave(QString fileToSave);
    void setLblInfo(QString lblInfo);
    void setCommand(QString command);
    void setDataSource(DataSource* dataSource);
    void setDataSource2(DataSource* dataSource2);
    void setDataSource3(DataSource* dataSource3);
    void setWorkerName(QString workerName);
    void setModel(Model* model);
    void setSliceTranslate(float sliceTranslate);

signals:
    void sliceChanged(float slice);
    void enableCuttingChanged(bool enableCutting);
    void fileToOpenChanged(QString fileToOpen);
    void fileToSaveChanged(QString fileToSave);
    void lblInfoChanged(QString lblInfo);
    void commandChanged(QString command);
    void dataSourceChanged(DataSource* dataSource);
    void dataSource2Changed(DataSource* dataSource2);
    void dataSource3Changed(DataSource* dataSource3);
    void workerNameChanged(QString workerName);
    void modelChanged(Model* model);
    void sliceTranslateChanged(float sliceTranslate);
};

class MyWorker : public SimulatorWorker
{
    Q_OBJECT

private:
    WorkerData* workerData = nullptr;
    Parameters* m_params = nullptr;
    Particles m_particles;
    Particles m_dataParticles;
    Spheres m_spheres;
    DTALikelihood m_likelihood;
    float m_porosity = 0;

    void constrainParticles(Spheres* spheres, Particles* extraList);
    void addParticleToSphere(Particle* p, Spheres *spheres, Particles* extraList);
    void openFile();
    void saveFile();
    bool manageCommands();
    void calculateStatistics();
    void saveStatistics();
    void calculateOctree();
    void calculateFractalDimension();


    void calculatePorosity();
    // This one calculates E(x) and sigma for the given model
    void calculateModelStatistics();

    // SimulatorWorker interface
    virtual void synchronizeSimulator(Simulator *simulator);
    virtual void work();

public:
    MyWorker() { }
    ~MyWorker() {
        m_particles.clear();
    }

    // SimulatorWorker interface
private:
    void synchronizeRenderer(Renderable *renderableObject);
    void calculateCurrentStatistics();
};

class MySimulator : public Simulator
{
    Q_OBJECT

    Q_PROPERTY(WorkerData* data READ data WRITE setData NOTIFY dataChanged)
    WorkerData* m_data = nullptr;

public:
    MySimulator();
    WorkerData* data() const;

public slots:
    void setData(WorkerData* data);

signals:
    void dataChanged(WorkerData* data);

protected:
    virtual SimulatorWorker *createWorker();
};

#endif // MYSIMULATOR_H
