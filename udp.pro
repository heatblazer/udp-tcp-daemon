TEMPLATE = subdirs

DISTFILES += README.md \
             recorder-config.xml \# the config file

SUBDIRS += \
    udp-server \
    udp-client \
    recorder-gui \
    crypto \
    plugin-test \
    plugin-test-2 \
    low-pass-filter

