#-------------------------------------------------
#
# Project created by QtCreator 2015-10-15T16:09:42
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = testsTextures
TEMPLATE = app
CONFIG *= qt opengl release
QT += opengl xml
CONFIG += c++11

SOURCES += main.cpp\
    mainwindow.cpp \
    monde.cpp \
    segment3d.cpp \
    triangle.cpp \
    bornes.cpp \
    recentreur.cpp \
    mondeviewer.cpp \
    threadouvrirmnt.cpp \
    polyligne.cpp \
    vagues.cpp

HEADERS  += mainwindow.h \
    monde.h \
    segment3d.h \
    triangle.h \
    bornes.h \
    recentreur.h \
    mondeviewer.h \
    threadouvrirmnt.h \
    polyligne.h \
    vagues.h

FORMS    += mainwindow.ui

unix {
    LIBS += -lQGLViewer
}
macx {
    QMAKE_LFLAGS += -F/Library/Frameworks
    INCLUDEPATH += /Library/Frameworks/QGLViewer.framework/Headers/
    LIBS += -framework QGLViewer
}
win32 {
    INCLUDEPATH *= D:\Qt\Libs
    LIBS *= -lQGLViewerd2 -LD:\Qt\Libs\QGLviewer #debug
    #LIBS *= -lQGLViewer2 -LD:\Qt\Libs\QGLviewer #release
}
