TEMPLATE = app
CONFIG += c++11

#CONFIG -=app_bundle
QT += qml quick widgets opengl openglextensions svg

INCLUDEPATH += /usr/local/include
LIBS += -L/usr/local/lib -lgsl

unix|win32: LIBS += -L$$PWD/../../build-GeometryLibrary-Desktop_Qt_5_5_1_clang_64bit-Release/ -lGeometryLibrary.1.0.0

INCLUDEPATH += $$PWD/../../GeometryLibrary
DEPENDPATH += $$PWD/../../GeometryLibrary


SOURCES += main.cpp \
    mysimulator.cpp \
    io.cpp \
    particle.cpp \
    QMLPlot/linegraph.cpp \
    QMLPlot/graph.cpp \
    QMLPlot/figure.cpp \
    distancetoatom.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)
include(../SimVis/library_deployment.pri)

HEADERS += \
    mysimulator.h \
    simplexnoise.h \
    perlin.h \
    CNoise.h \
    io.h \
    CSimplex.h \
    particle.h \
    QMLPlot/linegraph.h \
    QMLPlot/graph.h \
    QMLPlot/figure.h \
    distancetoatom.h
