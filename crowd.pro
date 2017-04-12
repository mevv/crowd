#-------------------------------------------------
#
# Project created by QtCreator 2017-04-10T11:22:36
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = crowd
TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    paint_widget.cpp \
    objects_pool.cpp \
    scene_object.cpp \
    agent.cpp \
    scene.cpp \
    calculator.cpp \
    obstacle.cpp

HEADERS  += mainwindow.h \
    paint_widget.h \
    objects_pool.h \
    scene_object.h \
    agent.h \
    scene.h \
    calculator.h \
    obstacle.h

FORMS    += mainwindow.ui
