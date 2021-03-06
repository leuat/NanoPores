#include "mysimulator.h"
#include <QDebug>
#include <iostream>
#include "io.h"
#include "GeometryLibrary/measures/distancetoatom.h"
#include "GeometryLibrary/noise.h"
#include "GeometryLibrary/models/octree.h"
#include "GeometryLibrary/measures/gofr.h"
#include "GeometryLibrary/measures/surfacearea.h"
using namespace std;

MyWorker::MyWorker() {
    SurfaceArea *surfaceArea = new SurfaceArea();
    m_measures.push_back(surfaceArea);
}

MySimulator::MySimulator()
{

}

WorkerData *MySimulator::data() const
{
    return m_data;
}

void MySimulator::setData(WorkerData *data)
{
    if (m_data == data)
        return;

    m_data = data;
    emit dataChanged(data);
}

SimulatorWorker *MySimulator::createWorker()
{
    return new MyWorker();
}

void MyWorker::addParticleToSphere(Particle* p, Spheres *spheres, Particles* extraList) {
    QVector3D color = p->getType().color;
    if (spheres!=nullptr) {
        spheres->colors().append(QColor(color.x()*255.0, color.y()*255.0, color.z()*255.0, 1));
        spheres->positions().append( m_particles.scalePos(p,5));
        spheres->scales().append(p->getType().size);
    }
    if (extraList!=nullptr) {
        extraList->getParticles().append(p);
    }
}

void MyWorker::openFile()
{
    if (workerData==nullptr) return;
    if (!workerData->fileToOpen().isEmpty()) {
        QUrl url = workerData->fileToOpen();
        qDebug() << workerData->workerName() << " will opening file " << url;
        m_particles.open(url.toLocalFile().toStdString().c_str());
        qDebug() << workerData->workerName() << " done opening file." << url;
        workerData->setFileToOpen("");
    }
}

void MyWorker::saveFile()
{
    if (workerData==nullptr) return;

    if (!workerData->fileToSave().isEmpty()) {
        QUrl url = workerData->fileToSave();
        Particles newList;
        constrainParticles(nullptr, &newList);
        newList.save(url.toLocalFile().toStdString().c_str());
        workerData->setFileToSave("");
    }
}

void MyWorker::calculateCurrentStatistics() {
    if (workerData==nullptr) return;
    if (m_particles.size()==0) return;

    Particles newList;
    constrainParticles(nullptr, &newList);

    QVector<QVector3D> points = newList.getQVector3DList();
    QVector<QVector3D> points2 = m_dataParticles.getQVector3DList();
    QElapsedTimer t;

    GOfR gr1;
    gr1.setCutoff(15); gr1.setNumBins(250);
    GOfR gr2;
    gr2.setCutoff(15); gr2.setNumBins(250);
    t.start();
    gr1.compute(points);
    gr2.compute(points2);
    gr1.setXRange(3.0, 1000);
    gr2.setXRange(3.0, 1000);

    qDebug() << "g of r finished after " << t.elapsed() << " ms";
    int numRandomVectors = 10000;
    float cutoff = 15;

    DistanceToAtom da;
    DistanceToAtom da2;
    da.setNumberOfRandomVectors(numRandomVectors);
    da2.setNumberOfRandomVectors(numRandomVectors);
    da.setCutoff(cutoff);
    da2.setCutoff(cutoff);
    t.restart();
    da.compute(points);
    da2.compute(points2);
    QVector<QPointF> hist1 = da.histogram(100);
    QVector<QPointF> hist2 = da2.histogram(100);
    qDebug() << "DTA finished after " << t.elapsed() << " ms";

//    hist1 = gr1.histogram(100);
//    hist2 = gr2.histogram(100);

    LGraph model;
    model.fromQVector(hist1);
    // model.normalizeArea();

    LGraph data;
    data.fromQVector(hist2);
    // data.normalizeArea();

    float chisq = LGraph::ChiSQ(data, model, 1.0);
//    qDebug() << "Chisq: " << chisq;

    workerData->dataSource2()->setPoints(hist1, false);
    workerData->dataSource3()->setPoints(hist2, false);

    for(Measure *measure : m_measures) {
        measure->compute(points, true);
        measure->compute(points2, true);
    }
}

bool MyWorker::manageCommands()
{
    m_dtaLikelihood.setNumberOfRandomVectors(30000);

    m_gOfRLikelihood.setCutoff(15);
    m_gOfRLikelihood.setNumberOfHistogramBins(100);

    m_likelihood = &m_gOfRLikelihood;
    // m_likelihood = &m_dtaLikelihood;
    if (!workerData->command().isEmpty()) {
        QStringList cmd = workerData->command().toLower().split(" ");
        QString command = cmd[0];
        if (command=="statistics") {
            m_likelihood->setOriginalInput(&m_particles);
            if (m_dataParticles.size()==0) {
                qDebug() << "ZERO particles in data input!";
                workerData->setCommand("");
                return false;
            }
           // m_likelihood->setDataInput(&m_dataParticles);

            m_likelihood->bruteForce1D(30, cmd[1], workerData->model());
        } else if(command == "current_statistics") {
            calculateCurrentStatistics();
            workerData->setCommand("");
        } else if (command=="likelihoodmontecarlo") {
            if (m_dataParticles.size()==0) {
                qDebug() << "ZERO particles in data input!";
                workerData->setCommand("");
                return false;
            }
            m_likelihood->monteCarlo(workerData->model(), 1000000, Likelihood::AnalysisAlgorithm::FullMonteCarlo);
        } else if (command=="loaddata") {
            QUrl url = cmd[1];
            m_dataParticles.open(url.toLocalFile().toStdString().c_str());
            m_likelihood->setDataInput(&m_dataParticles);
            qDebug() << "Loaded " << m_likelihood->dataParticles().size() << " data particles";
        } else if (command =="calculate_porosity") {
            calculatePorosity();
        } else if (command == "save_statistics") {
            saveStatistics();
        } else if (command == "calculate_octree_measure") {
            calculateOctree();
        } else if (command == "calculate_fractal_dimension") {
            calculateFractalDimension();
        } else if (command=="calculate_model_statistics") {
            m_likelihood->setOriginalInput(&m_particles);
            if (m_dataParticles.size()==0) {
                qDebug() << "ZERO particles in data input!";
                workerData->setCommand("");
                return false;
            }

            calculateModelStatistics(m_likelihood);
        } else {
            qDebug() << workerData->workerName() << "Could not parse command: " << cmd[0];
        }
        workerData->setCommand("");
        return true;
    }

    return true;
}

void MyWorker::calculateOctree() {
    if (workerData==nullptr)
        return;

    for (int i=0;i<=1;i++) {

        Octree oct;
        //oct.setThreshold(4);
        QVector<QVector3D> list;
        if (i==0) {
            m_dataParticles.appendToQVector3DList(list);

        }
        if (i==1) {
            Particles newList;
            constrainParticles(nullptr, &newList);
            newList.appendToQVector3DList(list);
//            list.resize(m_dataParticles.size());
        }

        oct.setPoints(list);
        oct.calculateBoundingbox();

        oct.setMaxDepth(10);
        oct.buildTree(false, false);
        QVector<QPointF> measure;
        oct.calculateOctreeMeasure(measure);
        for (int i=1;i<measure.size();i++) {
            if (measure[i].y()!=0)
//                measure[i].setY(log(measure[i].y()));
            measure[i].setY((measure[i].y()/pow(8,i)));
            //qDebug() << measure[i];
        }
        if (i==0)
            workerData->dataSource2()->setPoints(measure, true);
        if (i==1)
            workerData->dataSource3()->setPoints(measure, true);

      }


}

void MyWorker::calculateFractalDimension()
{
    Model *model = workerData->model();
    if(!model) return;
    model->start();

    model->calculateFractalDimension(0,157);
    model->stop();

}


void MyWorker::calculateStatistics()
{
    if (workerData==nullptr)
        return;
    Particles newList;
    // First, copy particles to single list
    if (m_particles.size()==0)
        return;

    constrainParticles(nullptr, &newList);
    QVector<QVector3D> points;
    newList.appendToQVector3DList(points);
    DistanceToAtom da;
    da.setNumberOfRandomVectors(4000);
    da.setCutoff(20);

    da.compute(points);
    QVector<QPointF> hist = da.histogram(100);

    workerData->dataSource2()->setPoints(hist, true);

    /*
    workerData->dataSource()->clear();
    for (int i=0;i<100;i++)
        workerData->dataSource()->addPoint(i, rand()%100);
*/
}

void MyWorker::saveStatistics()
{
    string dir = "";

    LGraph dataDTA = m_dtaLikelihood.calculateStatisticsDirect(m_dataParticles);
    Particles newList;
    constrainParticles(nullptr, &newList);
    LGraph modelDTA = m_dtaLikelihood.calculateStatisticsDirect(newList);

    dataDTA.SaveText(dir + "dta_data.txt");
    modelDTA.SaveText(dir +"dta_model.txt");
    m_dtaLikelihood.setOriginalInput(&m_particles);
    m_dtaLikelihood.modelAnalysis(50, workerData->model());

    while (m_dtaLikelihood.getDone()==false)
        if (m_dtaLikelihood.tick()) {
            m_dtaLikelihood.getStatistics().average().SaveText("model_average.txt");
            m_dtaLikelihood.getStatistics().sigma().SaveText("model_sigma.txt");
        }
}

void MyWorker::calculateModelStatistics(Likelihood *lh)
{
    lh->modelAnalysis(15, workerData->model());
}


void MyWorker::constrainParticles(Spheres* spheres, Particles* extraList) {
    if (workerData==nullptr)
        return;

    if (spheres!=nullptr) {
        spheres->colors().clear();
        spheres->scales().clear();
        spheres->positions().clear();
    }
    if (m_particles.size()==0) {
        return;
    }

    m_particles.calculateBoundingBox();
    if (workerData->enableCutting()) {
        Model *model = workerData->model();
        if(!model) return;

        const int numberOfParticles = m_particles.getParticles().size();
        QVector<bool> shouldBeAdded;
        shouldBeAdded.resize(numberOfParticles);
        memset(&shouldBeAdded[0], 0, shouldBeAdded.size()*sizeof(bool));

        model->start();
        // #pragma omp parallel for num_threads(8)
        float inside = 0;

        for(int i=0; i<numberOfParticles; i++) {
            Particle *particle = m_particles.getParticles()[i];
            const QVector3D position = particle->getPos();
            // QVector3D scaledPos = position/m_particles.getBoundsSize()*10;
            if (!(position[0] + m_particles.getBoundsSize()*workerData->sliceTranslate()<m_particles.getBoundsMin()[0]*workerData->slice() ||
                  position[0] + m_particles.getBoundsSize()*workerData->sliceTranslate()>m_particles.getBoundsMax()[0]*workerData->slice()))
            {
                if (!model->isInVoid(position)) {
                    shouldBeAdded[i] = true;
                    inside++;
                }
            }
        }
        model->stop();

//        qDebug() << "Actual porousity: " << (1-(inside/(float)numberOfParticles))*100 << "%";

        for(int i=0; i<numberOfParticles; i++) {
            Particle *pos = m_particles.getParticles()[i];
            if(shouldBeAdded[i])  addParticleToSphere(pos, spheres,extraList);
        }
        shouldBeAdded.clear();
    }
    else { // Just copy
        for (Particle* pos : m_particles.getParticles()) {
            addParticleToSphere(pos, spheres, extraList);
        }
    }
    if (spheres != nullptr) {
        QString porosity = "Porosity: " + QString("%1").arg((float)spheres->positions().length() / (float)m_particles.size()) + ", calculated: " + QString("%1").arg(m_porosity) +"\n";
        workerData->setLblInfo(porosity + "# particles: "+ QString("%1").arg(spheres->positions().size()));
    }
}

void MyWorker::calculatePorosity() {
    Particles newList;
    constrainParticles(nullptr, &newList);
    newList.calculatePorosity();
    m_porosity = newList.getCalculatedPorosity();
}


void MyWorker::synchronizeRenderer(Renderable *renderableObject) {
    Spheres *spheres = qobject_cast<Spheres*>(renderableObject);

    if(spheres &&  (m_particles.size()!=0)) {
        spheres->setColors(m_spheres.colors());
        spheres->setPositions(m_spheres.positions());
        spheres->setScales(m_spheres.scales());

        spheres->setDirty(true);
        return;
    }

}

void MyWorker::synchronizeSimulator(Simulator *simulator)
{
    MySimulator *mySimulator = qobject_cast<MySimulator*>(simulator);
    if(mySimulator) {
        workerData = mySimulator->data();
        openFile();
        saveFile();
        bool everythingOK = manageCommands();

        if(everythingOK) {
            if (m_likelihood->tick()) {
                workerData->dataSource()->setPoints(m_likelihood->likelihood().toQVector());
                workerData->dataSource2()->setPoints(m_likelihood->modelData().toQVector());
                workerData->dataSource3()->setPoints(m_likelihood->data().toQVector());
            }
            if (m_likelihood->getDone()){
                m_likelihood->setDone(false);
                workerData->dataSource2()->setPoints(m_likelihood->modelData().toQVector());
                qDebug() << "Min value: " << m_likelihood->getMinVal();
            }
        }
    }
}

void MyWorker::work()
{
    if (workerData!=nullptr) {
        constrainParticles(&m_spheres, nullptr);
    }
}

float WorkerData::slice() const
{
    return m_slice;
}
bool WorkerData::enableCutting() const
{
    return m_enableCutting;
}
QString WorkerData::fileToOpen() const
{
    return m_fileToOpen;
}
QString WorkerData::fileToSave() const
{
    return m_fileToSave;
}
QString WorkerData::lblInfo() const
{
    return m_lblInfo;
}
QString WorkerData::command() const
{
    return m_command;
}
DataSource *WorkerData::dataSource() const
{
    return m_dataSource;
}
DataSource *WorkerData::dataSource2() const
{
    return m_dataSource2;
}
DataSource *WorkerData::dataSource3() const
{
    return m_dataSource3;
}
QString WorkerData::workerName() const
{
    return m_workerName;
}
Model *WorkerData::model() const
{
    return m_model;
}
float WorkerData::sliceTranslate() const
{
    return m_sliceTranslate;
}
void WorkerData::allocate() {
    if (initialized)
        return;
    initialized = true;
}
void WorkerData::setSlice(float slice)
{
    if (m_slice == slice)
        return;

    m_slice = slice;
    emit sliceChanged(slice);
}
void WorkerData::setEnableCutting(bool enableCutting)
{
    if (m_enableCutting == enableCutting)
        return;

    m_enableCutting = enableCutting;
    emit enableCuttingChanged(enableCutting);
}
void WorkerData::setFileToOpen(QString fileToOpen)
{
    if (m_fileToOpen == fileToOpen)
        return;

    m_fileToOpen = fileToOpen;
    emit fileToOpenChanged(fileToOpen);
}
void WorkerData::setFileToSave(QString fileToSave)
{
    if (m_fileToSave == fileToSave)
        return;

    m_fileToSave = fileToSave;
    emit fileToSaveChanged(fileToSave);
}
void WorkerData::setLblInfo(QString lblInfo)
{
    if (m_lblInfo == lblInfo)
        return;

    m_lblInfo = lblInfo;
    emit lblInfoChanged(lblInfo);
}
void WorkerData::setCommand(QString command)
{
    if (m_command == command)
        return;

    m_command = command;
    emit commandChanged(command);
}
void WorkerData::setDataSource(DataSource *dataSource)
{
    if (m_dataSource == dataSource)
        return;

    m_dataSource = dataSource;
    emit dataSourceChanged(dataSource);
}
void WorkerData::setDataSource2(DataSource *dataSource2)
{
    if (m_dataSource2 == dataSource2)
        return;

    m_dataSource2 = dataSource2;
    emit dataSource2Changed(dataSource2);
}
void WorkerData::setDataSource3(DataSource *dataSource3)
{
    if (m_dataSource3 == dataSource3)
        return;

    m_dataSource3 = dataSource3;
    emit dataSource3Changed(dataSource3);
}
void WorkerData::setWorkerName(QString workerName)
{
    if (m_workerName == workerName)
        return;

    m_workerName = workerName;
    emit workerNameChanged(workerName);
}
void WorkerData::setModel(Model *model)
{
    if (m_model == model)
        return;

    m_model = model;
    emit modelChanged(model);
}
void WorkerData::setSliceTranslate(float sliceTranslate)
{
    if (m_sliceTranslate == sliceTranslate)
        return;

    m_sliceTranslate = sliceTranslate;
    emit sliceTranslateChanged(sliceTranslate);
}
