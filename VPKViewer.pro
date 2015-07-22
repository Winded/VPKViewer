#-------------------------------------------------
#
# Project created by QtCreator 2015-04-15T16:29:48
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VPKViewer
TEMPLATE = app
CONFIG += c++11

SOURCES +=\
    src/main.cpp \
    src/mainwindow.cpp \
    src/configmanager.cpp \
    src/processor.cpp \
    src/fileservice.cpp \
    src/fileobjectmodel.cpp \
    src/datawidgetbinder/datawidgetbinder.cpp

HEADERS  += \
    src/mainwindow.h \
    src/configmanager.h \
    src/processor.h \
    src/fileservice.h \
    src/fileobjectmodel.h \
    src/datawidgetbinder/datawidgetbinder.h

FORMS    += \
    src/mainwindow.ui
