#-------------------------------------------------
#
# Project created by QtCreator 2016-03-31T22:59:30
#
#-------------------------------------------------

QT       += core gui concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test_qt_RoundObjects
TEMPLATE = app


SOURCES += qt_RoundObjects.cpp\
        ../dialog.cpp \
    ../log.cpp \
    ../movement.cpp \
    ../statwindow.cpp

HEADERS  += ../dialog.hpp \
    ../log.hpp \
    ../movement.hpp \
    ../statwindow.hpp

FORMS    += ../dialog.ui

CONFIG += c++11
QMAKE_CXXFLAGS += -std=c++11
QMAKE_LFLAGS += -std=c++11
