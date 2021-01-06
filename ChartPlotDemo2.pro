#-------------------------------------------------
#
# Project created by QtCreator 2021-01-06T13:48:21
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ChartPlotDemo2
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    LineChart.cpp \
    DataSourceProvider.cpp \
    StateMachine.cpp

HEADERS  += mainwindow.h \
    LineChart.h \
    DataSourceProvider.h \
    StateMachine.h

FORMS    += mainwindow.ui
