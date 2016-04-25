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
    GeometryLibrary/likelihood/lgraph.cpp \
    GeometryLibrary/particle.cpp \
    GeometryLibrary/distancetoatom.cpp \
    GeometryLibrary/dtalikelihood.cpp \
    GeometryLibrary/likelihood/likelihood.cpp \
    GeometryLibrary/misc/util.cpp \
    GeometryLibrary/models/model.cpp \
    GeometryLibrary/models/multifractalmodel.cpp \
    GeometryLibrary/noise.cpp \
    GeometryLibrary/perlin.cpp \
    GeometryLibrary/simplex.cpp \
    GeometryLibrary/simplexnoise.cpp \
    GeometryLibrary/likelihood/graphstatistics.cpp \
    GeometryLibrary/parameters.cpp \
    GeometryLibrary/models/regularnoisemodel.cpp \
    GeometryLibrary/models/xyzmodel.cpp \
    GeometryLibrary/misc/cutil.cpp \
    datasource.cpp \
    GeometryLibrary/models/octree.cpp \
<<<<<<< HEAD
    GeometryLibrary/logger.cpp
=======
    GeometryLibrary/logger.cpp \
>>>>>>> 403f2db9e1c1cf19ac7a5f734f7b1a5b40fcad83

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)
include(../SimVis/library_deployment.pri)

HEADERS += \
    mysimulator.h \
    io.h \
    particle.h \
    distancetoatom.h \
    dtalikelihood.h \
    GeometryLibrary/likelihood/lgraph.h \
    GeometryLibrary/likelihood/likelihood.h \
    GeometryLibrary/likelihood/spline.h \
    GeometryLibrary/misc/util.h \
    GeometryLibrary/models/model.h \
    GeometryLibrary/models/multifractalmodel.h \
    GeometryLibrary/noise.h \
    GeometryLibrary/perlin.h \
    GeometryLibrary/simplex.h \
    GeometryLibrary/simplexnoise.h \
    GeometryLibrary/likelihood/graphstatistics.h \
    GeometryLibrary/parameters.h \
    GeometryLibrary/models/models.h \
    GeometryLibrary/models/regularnoisemodel.h \
    GeometryLibrary/models/xyzmodel.h \
    GeometryLibrary/misc/cinifile.h \
    GeometryLibrary/misc/cutil.h \
    datasource.h \
    GeometryLibrary/models/octree.h \
    GeometryLibrary/misc/random.h \
<<<<<<< HEAD
    GeometryLibrary/logger.h
=======
    GeometryLibrary/logger.h \
    GeometryLibrary/dtalikelihood.h \
    GeometryLibrary/particle.h \
    GeometryLibrary/distancetoatom.h
>>>>>>> 403f2db9e1c1cf19ac7a5f734f7b1a5b40fcad83

DISTFILES += \
    GeometryLibrary/README.md
