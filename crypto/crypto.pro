#-------------------------------------------------
#
# Project created by QtCreator 2016-10-31T15:16:53
# Crypto will be used by recorder and player so
# it will be used as a library
#-------------------------------------------------

QT       += core

TARGET = crypto
TEMPLATE = app

DEFINES += CRYPTO_LIBRARY

SOURCES += crypto.cpp \
    main.cpp

HEADERS += crypto.h\
        crypto_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

DISTFILES += \
    rfc2104.txt
