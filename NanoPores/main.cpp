#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include "mysimulator.h"
int main(int argc, char *argv[])
{
    qmlRegisterType<MySimulator>("MySimulator", 1, 0, "MySimulator");
    qmlRegisterType<WorkerData>("WorkerData", 1, 0, "WorkerData");

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}

