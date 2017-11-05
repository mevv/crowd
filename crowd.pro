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
    obstacle.cpp \
    engine.cpp \
    exit.cpp \
    entry.cpp \
    plan_builder.cpp \
    spawn_zone.cpp \
    crowd_parameters.cpp \
    statistics.cpp \
    general_builder.cpp \
    json_manager.cpp \
    checkpoint.cpp \
    map_search_node.cpp \
    a_star.cpp \
    leePathfinder.cpp \
    lee.cpp

HEADERS  += mainwindow.h \
    paint_widget.h \
    objects_pool.h \
    scene_object.h \
    agent.h \
    scene.h \
    calculator.h \
    obstacle.h \
    engine.h \
    exit.h \
    entry.h \
    plan_builder.h \
    spawn_zone.h \
    crowd_parameters.h \
    statistics.h \
    general_builder.h \
    json_manager.h \
    a_star.h \
    stlastar.h \
    fsa.h \
    is_in_polygon.h \
    checkpoint.h \
    map_search_node.h \
    map_container.h \
    leePathfinder.h \
    lee.h

FORMS    += mainwindow.ui \
    crowdparameters.ui \
