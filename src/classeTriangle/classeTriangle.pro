TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    triangle.cpp

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    triangle.h

