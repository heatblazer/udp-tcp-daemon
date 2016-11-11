QT += core
QT += xml
QT += network
QT -= gui

CONFIG += c++11

TARGET = recd2
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    server.cpp \
    sapplication.cpp \
    utils/recorder-config.cpp \
    recorder.cpp \
    unix/daemon.cpp \
    utils/wav-writer.cpp \
    utils/qwave-writer.cpp \
    utils/writer.cpp \
    plugin-manager.cpp

HEADERS += \
    server.h \
    sapplication.h \
    utils/recorder-config.h \
    types.h \
    recorder.h \
    unix/daemon.h \
    utils/wav-writer.h \
    utils/qwave-writer.h \
    utils/writer.h \
    utils/wav-writer-iface.h \
    plugin-manager.h


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
    -std=gnu11

# custom macrodefs for the tests
 QMAKE_CXXFLAGS += -DPLUGIN_TEST
                   #-DTEST
                  #-DXML_TEST
                   #-DCRYPTO_TEST


# redefine a Makefile INSTALL_PROGRAM variable. We want to instal wit SUID bit
QMAKE_INSTALL_PROGRAM = install -m 4755 -p

target.path = /usr/bin
INSTALLS += target
