#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include "mysimulator.h"
#include "QMLPlot/figure.h"
#include "QMLPlot/linegraph.h"

int main(int argc, char *argv[])
{
    qmlRegisterType<MySimulator>("MySimulator", 1, 0, "MySimulator");
    qmlRegisterType<WorkerData>("WorkerData", 1, 0, "WorkerData");
    qmlRegisterType<Figure>("QMLPlot", 1,0,"Figure");
    qmlRegisterType<LineGraph>("QMLPlot", 1,0,"LineGraph");
    qmlRegisterType<LineGraphDataSource>("QMLPlot", 1,0,"LineGraphDataSource");


    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}

