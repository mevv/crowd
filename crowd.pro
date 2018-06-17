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

linux {
    QMAKE_CXXFLAGS += -std=c++11 -W -Wall -Wextra -pedantic
    message("Linux system")
}

SOURCES += src/main.cpp\
        src/mainwindow.cpp \
    src/paint_widget.cpp \
    src/objects_pool.cpp \
    src/scene_object.cpp \
    src/agent.cpp \
    src/scene.cpp \
    src/calculator.cpp \
    src/obstacle.cpp \
    src/engine.cpp \
    src/exit.cpp \
    src/entry.cpp \
    src/plan_builder.cpp \
    src/spawn_zone.cpp \
    src/crowd_parameters.cpp \
    src/statistics.cpp \
    src/general_builder.cpp \
    src/json_manager.cpp \
    src/checkpoint.cpp \
    src/map_search_node.cpp \
    src/a_star.cpp \
    src/leePathfinder.cpp \
    src/lee.cpp

HEADERS  += src/mainwindow.h \
    src/paint_widget.h \
    src/objects_pool.h \
    src/scene_object.h \
    src/agent.h \
    src/scene.h \
    src/calculator.h \
    src/obstacle.h \
    src/engine.h \
    src/exit.h \
    src/entry.h \
    src/plan_builder.h \
    src/spawn_zone.h \
    src/crowd_parameters.h \
    src/statistics.h \
    src/general_builder.h \
    src/json_manager.h \
    src/a_star.h \
    src/stlastar.h \
    src/fsa.h \
    src/is_in_polygon.h \
    src/checkpoint.h \
    src/map_search_node.h \
    src/map_container.h \
    src/leePathfinder.h \
    src/lee.h

FORMS    += src/mainwindow.ui \
    src/crowdparameters.ui \
