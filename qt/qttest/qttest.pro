#-------------------------------------------------
#
# Project created by QtCreator 2015-06-25T15:14:20
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qttest
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    logindialog.cpp \
    appitem.cpp

HEADERS  += mainwindow.h \
    logindialog.h \
    appitem.h

FORMS    += mainwindow.ui \
    logindialog.ui
