TEMPLATE = app
CONFIG += c++11
QMAKE_CXXFLAGS +=
QMAKE_LFLAGS +=
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
    distancetoatom.cpp \
    dtalikelihood.cpp \
    GeometryLibrary/likelihood/lgraph.cpp \
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
    GeometryLibrary/models/octree.cpp

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
    GeometryLibrary/models/octree.h

DISTFILES += \
    GeometryLibrary/README.md
