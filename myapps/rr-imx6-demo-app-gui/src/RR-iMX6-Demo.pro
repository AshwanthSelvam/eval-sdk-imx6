#-------------------------------------------------
#
# Project created by QtCreator 2013-08-07T09:26:06
#
#-------------------------------------------------

QT       += core gui

TARGET = RR-iMX6-Demo
TEMPLATE = app


SOURCES += main.cpp\
        rr_imx6_demo.cpp \
    tools.cpp \
    gst_pipeline.cpp \
    gst_factory.cpp \
    pipelines.cpp

HEADERS  += rr_imx6_demo.h \
    tools.h \
    gst_pipeline.h \
    gst_factory.h \
    pipelines.h

FORMS    += rr_imx6_demo.ui

RESOURCES += \
    resources.qrc

CONFIG += qdbus
CONFIG += console

# Install set
target.path = /usr/bin

INSTALLS += target
