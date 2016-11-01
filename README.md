udp-server
==========

A wav recorder over udp.


## Build
   Added the dummy project for the gui interface of the recorder.

## Usage

    $ ./udp-server -d # as daemon
    $ ./udp-server    # as normal app

## TODO
    1. I have to remove the udp-client project when I am done with tests.
    2. I have to encrypt or add a check sum to the samples.
    3. I have to provide install options and "install howto" in build section.

## NOTES
    31.10.2016: now correctly records the samples from the incomming device.
    31.10.2016: added libary project for crypting files or adding checksums.
    01.11.2016: added recorder-config.xml for more complex configure.
    01.11.2016: changed the logic and connection between `recorder` and `server`.

