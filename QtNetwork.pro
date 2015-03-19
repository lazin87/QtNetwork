#-------------------------------------------------
#
# Project created by QtCreator 2015-03-15T19:48:04
#
#-------------------------------------------------

QT       += core gui
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtNetwork
TEMPLATE = app


SOURCES += main.cpp\
        blockingclient.cpp \
    fortunethread.cpp \
    httpwindow.cpp

HEADERS  += blockingclient.h \
    fortunethread.h \
    httpwindow.h

DISTFILES +=

FORMS += \
    authenticationdialog.ui
