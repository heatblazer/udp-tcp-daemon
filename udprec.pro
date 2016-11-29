TEMPLATE = subdirs

DISTFILES += README.md \
             recorder-config.xml \# the config file
             recd2.pri

SUBDIRS += \
    udp-server \
    udp-client \
    DFT   #discrete fourier transform


