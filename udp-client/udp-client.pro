QT += core
QT -= gui
QT += network

CONFIG += c++11

TARGET = udp-client
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    client.cpp \
    wav-writer.cpp

HEADERS += \
    client.h \
    defs.h \
    wav-writer.h

DISTFILES += \
    ../assets/ring-0db.wav
