#-------------------------------------------------
#
# Project created by QtCreator 2015-04-15T16:29:48
#
#-------------------------------------------------

QT       += core gui

QMAKE_CXXFLAGS += -Wunused-parameter

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VPKViewer
TEMPLATE = app
CONFIG += c++11

SOURCES +=\
    src/main.cpp \
    src/mainwindow.cpp \
    src/processor.cpp \
    src/fileservice.cpp \
    src/fileobjectmodel.cpp \
    src/datawidgetbinder/datawidgetbinder.cpp \
    src/configdialog.cpp \
    src/configservice.cpp \
    src/configmodel.cpp

HEADERS  += \
    src/mainwindow.h \
    src/processor.h \
    src/fileservice.h \
    src/fileobjectmodel.h \
    src/datawidgetbinder/datawidgetbinder.h \
    src/configdialog.h \
    src/configservice.h \
    src/configmodel.h

FORMS    += \
    src/mainwindow.ui \
    src/configdialog.ui
