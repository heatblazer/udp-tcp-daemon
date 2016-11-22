TEMPLATE = subdirs

DISTFILES += README.md \
             recorder-config.xml \# the config file

SUBDIRS += \
    udp-server \
    udp-client \
    recorder-gui \
    crypto \
    DFT   #discrete fourier transform

