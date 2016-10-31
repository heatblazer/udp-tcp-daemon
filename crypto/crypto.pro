#-------------------------------------------------
#
# Project created by QtCreator 2016-10-31T15:16:53
#
#-------------------------------------------------

QT       -= core gui

TARGET = crypto
TEMPLATE = lib

DEFINES += CRYPTO_LIBRARY

SOURCES += crypto.cpp

HEADERS += crypto.h\
        crypto_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
