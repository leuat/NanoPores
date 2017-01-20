TEMPLATE = app
CONFIG += c++11
QMAKE_CXXFLAGS += -fopenmp -g
QMAKE_LFLAGS += -fopenmp
QT += qml quick widgets opengl openglextensions charts svg

INCLUDEPATH += /usr/local/include
LIBS += -L/usr/local/lib -lgsl
LIBS += -L/opt/intel/lib
SOURCES += main.cpp \
    mysimulator.cpp \
    io.cpp \
    GeometryLibrary/likelihood/lgraph.cpp \
    GeometryLibrary/likelihood/gofrlikelihood.cpp \
    GeometryLibrary/likelihood/particlelikelihood.cpp \
    GeometryLibrary/likelihood/dtalikelihood.cpp \
    GeometryLibrary/likelihood/graphstatistics.cpp \
    GeometryLibrary/likelihood/likelihood.cpp \
    GeometryLibrary/misc/util.cpp \
    GeometryLibrary/particle.cpp \
    GeometryLibrary/models/model.cpp \
    GeometryLibrary/models/multifractalmodel.cpp \
    GeometryLibrary/noise.cpp \
    GeometryLibrary/perlin.cpp \
    GeometryLibrary/simplex.cpp \
    GeometryLibrary/simplexnoise.cpp \
    GeometryLibrary/parameters.cpp \
    GeometryLibrary/models/regularnoisemodel.cpp \
    GeometryLibrary/models/xyzmodel.cpp \
    GeometryLibrary/misc/cutil.cpp \
    datasource.cpp \
    GeometryLibrary/models/octree.cpp \
    GeometryLibrary/logger.cpp \
    GeometryLibrary/measures/measure.cpp \
    GeometryLibrary/measures/distancetoatom.cpp \
    GeometryLibrary/misc/grid.cpp \
    GeometryLibrary/misc/distancetoatommap.cpp \
    GeometryLibrary/misc/points.cpp \
    GeometryLibrary/measures/poresizedistribution.cpp \
    GeometryLibrary/models/mkiimodel.cpp \
    GeometryLibrary/measures/gofr.cpp \
    GeometryLibrary/measures/neighborlist.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)
include(../SimVis/library_deployment.pri)

HEADERS += \
    mysimulator.h \
    io.h \
    GeometryLibrary/likelihood/lgraph.h \
    GeometryLibrary/likelihood/likelihood.h \
    GeometryLibrary/likelihood/spline.h \
    GeometryLibrary/likelihood/gofrlikelihood.h \
    GeometryLibrary/likelihood/particlelikelihood.h \
    GeometryLibrary/likelihood/graphstatistics.h \
    GeometryLibrary/likelihood/dtalikelihood.h \
    GeometryLibrary/misc/util.h \
    GeometryLibrary/models/model.h \
    GeometryLibrary/models/multifractalmodel.h \
    GeometryLibrary/noise.h \
    GeometryLibrary/perlin.h \
    GeometryLibrary/simplex.h \
    GeometryLibrary/simplexnoise.h \
    GeometryLibrary/parameters.h \
    GeometryLibrary/models/models.h \
    GeometryLibrary/models/regularnoisemodel.h \
    GeometryLibrary/models/xyzmodel.h \
    GeometryLibrary/misc/cinifile.h \
    GeometryLibrary/misc/cutil.h \
    datasource.h \
    GeometryLibrary/models/octree.h \
    GeometryLibrary/misc/random.h \
    GeometryLibrary/logger.h \
    GeometryLibrary/particle.h \
    GeometryLibrary/measures/measure.h \
    GeometryLibrary/measures/distancetoatom.h \
    GeometryLibrary/misc/grid.h \
    GeometryLibrary/misc/distancetoatommap.h \
    GeometryLibrary/misc/points.h \
    GeometryLibrary/measures/poresizedistribution.h \
    GeometryLibrary/models/mkiimodel.h \
    GeometryLibrary/measures/gofr.h \
    GeometryLibrary/measures/neighborlist.h
DISTFILES += \
    GeometryLibrary/README.md
