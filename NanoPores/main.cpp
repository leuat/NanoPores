#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include "mysimulator.h"
#include "datasource.h"
#include "GeometryLibrary/parameters.h"
#include "GeometryLibrary/models/models.h"
#include "GeometryLibrary/models/mkiimodel.h"
int main(int argc, char *argv[])
{
    qmlRegisterType<MySimulator>("MySimulator", 1, 0, "MySimulator");
    qmlRegisterType<WorkerData>("WorkerData", 1, 0, "WorkerData");
    qmlRegisterType<DataSource>("DataSource", 1, 0, "DataSource");
    qmlRegisterUncreatableType<Model>("GeometryLibrary", 1, 0, "Model",
                                          "Cannot create abstract type Model. This must be subclassed.");
    qmlRegisterType<RegularNoiseModel>("GeometryLibrary", 1, 0, "RegularNoiseModel");
    qmlRegisterType<MultiFractalModel>("GeometryLibrary", 1, 0, "MultiFractalModel");
    qmlRegisterType<MKIIModel>("GeometryLibrary", 1, 0, "MKIIModel");
    qmlRegisterType<Parameters>("GeometryLibrary",1,0,"Parameters");
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}


