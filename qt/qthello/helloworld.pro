#-------------------------------------------------
#
# Project created by QtCreator 2015-06-24T16:08:48
#
#-------------------------------------------------

QT       += core gui webkit

TARGET = helloworld
TEMPLATE = app


SOURCES += main.cpp\
    webshellview.cpp \
    logindialog.cpp \
    settingdialog.cpp \
    config.cpp

HEADERS  += webshellview.h \
    logindialog.h \
    settingdialog.h \
    config.h

FORMS    += webshellview.ui \
    logindialog.ui \
    settingdialog.ui

RESOURCES += \
    images.qrc
