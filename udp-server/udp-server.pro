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
    wav-writer.cpp \
    sapplication.cpp \
    daemon.cpp \
    config.cpp \
    utils/recorder-config.cpp

HEADERS += \
    server.h \
    writer.h \
    wav-writer.h \
    sapplication.h \
    daemon.h \
    config.h \
    utils/recorder-config.h


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

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../crypto/release/ -lcrypto
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../crypto/debug/ -lcrypto
else:unix: LIBS += -L$$OUT_PWD/../crypto/ -lcrypto

INCLUDEPATH += $$PWD/../crypto
DEPENDPATH += $$PWD/../crypto
