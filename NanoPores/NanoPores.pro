TEMPLATE = app
CONFIG += c++11

#CONFIG -=app_bundle
QT += qml quick widgets opengl openglextensions

SOURCES += main.cpp \
    mysimulator.cpp \
    simplexnoise.cpp \
    perlin.cpp \
    CNoise.cpp \
    io.cpp \
    particle.cpp

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
    particle.h
