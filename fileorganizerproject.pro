#-------------------------------------------------
#
# Project created by QtCreator 2016-04-05T14:40:32
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = fileorganizerproject
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mydialogue.cpp \
    optthread.cpp

HEADERS  += mainwindow.h \
    mydialogue.h \
    optthread.h

FORMS    += mainwindow.ui \
    mydialogue.ui

#QMAKE_CXXFLAGS += -fopenmp
#QMAKE_LFLAGS +=  -fopenmp
QMAKE_CFLAGS_DEBUG += -fopenmp
