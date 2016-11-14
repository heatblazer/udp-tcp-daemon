TEMPLATE = lib
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    sniffer.cpp

HEADERS += \
    plugin-interface.h

LIBS += -lpcap \
        -lpthread

