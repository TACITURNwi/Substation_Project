#-------------------------------------------------
#
# Project created by QtCreator 2019-03-20T20:46:38
#
#-------------------------------------------------

QT       += core
QT       += serialport
QT       += network
QT       -= gui


TARGET = Holder_Laser
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    connectwithpc.cpp \
    holder.cpp \
    laser.cpp \
    protocol.cpp \
    serialport.cpp \
    manager.cpp \
    gyroscope.cpp \
    usart_ctrl_api.c

HEADERS += \
    connectwithpc.h \
    holder.h \
    laser.h \
    protocol.h \
    serialport.h \
    manager.h \
    usart_ctrl_api.h \
    gyroscope.h

CONFIG += C++11
