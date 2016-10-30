TEMPLATE = subdirs

DISTFILES += assets/1.wav \
        README.md \
        packet_parser.c \ # hint to capture utilities
        server_udp.c    # hint for unix sockerts

SUBDIRS += \
    udp-server \
    udp-client \
    recorder-gui
