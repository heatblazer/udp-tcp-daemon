QT += core
QT += network
QT -= gui

CONFIG += c++11

TARGET = udp-server
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    server.cpp \
    writer.cpp \
    wav-writer.cpp \
    sapplication.cpp \
    daemon.cpp

HEADERS += \
    server.h \
    writer.h \
    wav-writer.h \
    sapplication.h \
    daemon.h
