#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include "mysimulator.h"
#include "datasource.h"
#include "GeometryLibrary/models/noiseparameters.h"
int main(int argc, char *argv[])
{
    qmlRegisterType<MySimulator>("MySimulator", 1, 0, "MySimulator");
    qmlRegisterType<WorkerData>("WorkerData", 1, 0, "WorkerData");
    qmlRegisterType<NoiseParameters>("GeometryLibrary",1,0,"NoiseParameters");
    qmlRegisterType<DataSource>("DataSource", 1, 0, "DataSource");
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}


