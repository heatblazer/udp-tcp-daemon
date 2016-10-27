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
    daemon.cpp \
    config.cpp

HEADERS += \
    server.h \
    writer.h \
    wav-writer.h \
    sapplication.h \
    daemon.h \
    defs.h \
    config.h


QMAKE_CFLAGS += \
    -Wextra \
    -pedantic \
    -Wmissing-braces \
    -Wparentheses \
    -Wsequence-point \
    -Wswitch \
    -Wuninitialized \
    -Wcast-qual \
    -Wlogical-op \
    -Wnormalized=nfkc \
    -std=gnu11 \


# redefine a Makefile INSTALL_PROGRAM variable. We want to instal wit SUID bit
QMAKE_INSTALL_PROGRAM = install -m 4755 -p

target.path = /usr/bin
INSTALLS += target
