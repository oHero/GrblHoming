#-------------------------------------------------
#
# Project created by QtCreator 2014-05-10T23:04:55
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = TestsGrblController
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_preloadtest.cpp \
    tst_positemtest.cpp \
    testAll.cpp \
    mock_rs232.cpp \
    ../grblinterface.cpp \
    ../coord3d.cpp \
    ../controlparams.cpp \
    ../atomicintbool.cpp \
    ../cmdresponse.cpp \
    ../positem.cpp \
    ../grblcontrol.cpp \
    tst_grblcontroltest.cpp

DEFINES += QTTEST=1

HEADERS += \
    tst_positemtest.h \
    tst_preloadtest.h \
    ../grblinterface.h \
    ../grblcontrol.h \
    tst_grblcontroltest.h \
    ../rs232.h
