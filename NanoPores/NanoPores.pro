TEMPLATE = app
CONFIG += c++11
QMAKE_CXXFLAGS += -fopenmp
QMAKE_LFLAGS += -fopenmp
#QMAKE_CXXFLAGS += -O3 -ftree-vectorize -msse2 -ftree-vectorizer-verbose=5

#CONFIG -=app_bundle
QT += qml quick widgets opengl openglextensions charts svg

INCLUDEPATH += /usr/local/include
LIBS += -L/usr/local/lib -lgsl
LIBS += -L/opt/intel/lib
SOURCES += main.cpp \
    mysimulator.cpp \
    io.cpp \
    particle.cpp \
    QMLPlot/linegraph.cpp \
    QMLPlot/graph.cpp \
    QMLPlot/figure.cpp \
    distancetoatom.cpp \
    dtalikelihood.cpp \
    GeometryLibrary/likelihood/lgraph.cpp \
    GeometryLibrary/likelihood/likelihood.cpp \
    GeometryLibrary/misc/util.cpp \
    GeometryLibrary/models/model.cpp \
    GeometryLibrary/models/multifractalmodel.cpp \
    GeometryLibrary/models/multifractalparameters.cpp \
    GeometryLibrary/models/noiseparameters.cpp \
    GeometryLibrary/models/parameters.cpp \
    GeometryLibrary/models/regularmodel.cpp \
    GeometryLibrary/geometrylibrary.cpp \
    GeometryLibrary/noise.cpp \
    GeometryLibrary/perlin.cpp \
    GeometryLibrary/simplex.cpp \
    GeometryLibrary/simplexnoise.cpp \
    GeometryLibrary/likelihood/graphstatistics.cpp \
    datasource.cpp

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
    distancetoatom.h \
    dtalikelihood.h \
    GeometryLibrary/likelihood/lgraph.h \
    GeometryLibrary/likelihood/likelihood.h \
    GeometryLibrary/likelihood/spline.h \
    GeometryLibrary/misc/util.h \
    GeometryLibrary/models/model.h \
    GeometryLibrary/models/multifractalmodel.h \
    GeometryLibrary/models/multifractalparameters.h \
    GeometryLibrary/models/noiseparameters.h \
    GeometryLibrary/models/parameters.h \
    GeometryLibrary/models/regularmodel.h \
    GeometryLibrary/geometrylibrary.h \
    GeometryLibrary/geometrylibrary_global.h \
    GeometryLibrary/noise.h \
    GeometryLibrary/perlin.h \
    GeometryLibrary/simplex.h \
    GeometryLibrary/simplexnoise.h \
    GeometryLibrary/likelihood/graphstatistics.h \
    datasource.h
