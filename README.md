Recorder
========

A wav recorder over udp streams.


## Build
   Added the dummy project for the gui interface of the recorder.

## Usage

    $ ./udp-server -d -c <path-to-conf-file>        # as daemon
    $ ./udp-server -c <path-to-conf-file>           # as normal app
    $ ./udp-server --config <path-to-conf-file>     # as normal app


## TODO
    1. I have to remove the udp-client project when I am done with tests.
    2. I have to encrypt or add a check sum to the samples.
    3. I have to provide install options and "install howto" in build section.
    4. Crypto project and lib for hashing or crypting is still a stub.
    5. GUI for the recorder is stub yet.
    6. I have to add Build/Install instructions and options to the BUILD section.
    7. Daemon logging must support sys/log functionality. Not done yet.

## NOTES
    31.10.2016: now correctly records the samples from the incomming device.
    31.10.2016: added libary project for crypting files or adding checksums.
    01.11.2016: added recorder-config.xml for more complex configure.
    01.11.2016: changed the logic and connection between `recorder` and `server`.
    02.11.2016: more WIP and concept fixes to the recorder and server.
    02.11.2016: improved configure of the application.
    02.11.2016: added QWav class based on QFile for writing WAV, unimplemented.
    02.11.2016: fixed a bug with the daemon recorder. But still more to be done.
    03.11.2016: added 24 hour test to the server, with custom client.
    03.11.2016: passed an 1 hour record test with packet sent at 1000 msec.
    08.11.2016: passed 24 and 72 h tests from my client program.
    08.11.2016: passed sine wave test from the device.
    08.11.2016: have a packet loss per random second or two.

***
<ilianzapryanov@abv.bg>

И.Z.
