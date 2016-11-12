TEMPLATE = subdirs

DISTFILES += assets/1.wav \
        assets/test.wav \ # this file is written with my wav library
        README.md \
        packet_parser.c \ # hint to capture utilities
        server_udp.c  \  # hint for Daemon sockerts
        recorder-config.xml \# the config file
        rfc2104.txt \
        valgrind.png

SUBDIRS += \
    udp-server \
    udp-client \
    recorder-gui \
    crypto \
    plugin-test \
    plugin-test-2

