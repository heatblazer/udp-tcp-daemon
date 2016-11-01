TEMPLATE = subdirs

DISTFILES += assets/1.wav \
        assets/test.wav \ # this file is written with my wav library
        README.md \
        packet_parser.c \ # hint to capture utilities
        server_udp.c  \  # hint for unix sockerts
        recorder-config.xml # the config file

SUBDIRS += \
    udp-server \
    udp-client \
    recorder-gui \
    crypto
