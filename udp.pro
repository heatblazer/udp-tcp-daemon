TEMPLATE = subdirs

DISTFILES += assets/1.wav \
        README.md \
        packet_parser.c

SUBDIRS += \
    udp-server \
    udp-client
