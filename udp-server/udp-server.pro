QT += core
QT += xml
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
    sapplication.cpp \
    config.cpp \
    utils/recorder-config.cpp \
    recorder.cpp \
    unix/daemon.cpp \
    utils/wav-writer.cpp

HEADERS += \
    server.h \
    writer.h \
    sapplication.h \
    config.h \
    utils/recorder-config.h \
    types.h \
    recorder.h \
    unix/daemon.h \
    utils/wav-writer.h


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
QMAKE_CXXFLAGS += -DTEST \
                  -DXML_TEST
                   #-DCRYPTO_TEST


# redefine a Makefile INSTALL_PROGRAM variable. We want to instal wit SUID bit
QMAKE_INSTALL_PROGRAM = install -m 4755 -p

target.path = /usr/bin
INSTALLS += target

#win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../crypto/release/ -lcrypto
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../crypto/debug/ -lcrypto
#else:unix: LIBS += -L$$OUT_PWD/../crypto/ -lcrypto

#INCLUDEPATH += $$PWD/../crypto
#DEPENDPATH += $$PWD/../crypto
