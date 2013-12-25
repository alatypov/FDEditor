#-------------------------------------------------
#
# Project created by QtCreator 2013-12-17T07:57:17
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FDEditor
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    diagramscene.cpp \
    diagramitem.cpp \
    infodialog.cpp \
    line.cpp \
    linepoint.cpp \
    arrows.cpp \
    diagramtextitem.cpp

HEADERS  += mainwindow.h \
    diagramitem.h \
    diagramscene.h \
    line.h \
    helper.h \
    infodialog.h \
    ipad.h \
    arrows.h \
    diagramtextitem.h \
    linepoint.h \
    iojson.h



RESOURCES += \
    diagramscene.qrc
