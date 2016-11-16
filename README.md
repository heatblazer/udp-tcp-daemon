Recorder
========

A wav recorder over udp streams.

Player
======

A wav playback with waveform for listening the records.


## Build
   Added the dummy project for the gui interface of the recorder.

## Usage

    $ ./recd2 -d -c <path-to-conf-file>        # as daemon
    $ ./recd2 -c <path-to-conf-file>           # as normal app
    $ ./recd2 --config <path-to-conf-file>     # as normal app

## Config File

    HotSwap: maxSize in bytes or interval with timer.
    ChannelCount: unused.
    Wave: setup wav file - see the config file.
    Record: setup directories and other stuff for the recorder.
    Network: transport - can be udp or tcp, port to bind to.
    Log: name and path to log - unused for now.
    HeartBeat: send a keep alive packet to client if needed or to port.
    Plugin: name of the plugin, order - priority, enabled - disabled, path - fullpath.

    See the recorder-config.xml for example.
    The release will be preloaded with default settings if no options are given, or
    not proper setup is detected.
## Plugin API

    The program supports plugable programs, that can be loaded by describing them in the
    config file. When a programmer wants to support the program with a plugin, she must
    provide that interface:
        #ifdef __cplusplus
        extern "C"{
        #endif
        struct interface_t
        {
            void (*init)();
            int (*put_ndata)(void* data, int len);
            int (*put_data)(void* data);
            void* (*get_data)(void);
            void (*deinit)();
            int (*main_proxy)(int, char**);
        };

        const struct interface_t* get_interface();
        #ifdef __cplusplus
        }
        #endif

    In brief, I expect the plugin to be able to init and deinit itself, and for now to
    be able to put and get some unknown data, and also to be able to support the C main
    function as the proxy I`ve preserved. So the main can be passed to the plugin. When
    I decide to change the interface, the contributors will be informed.

## TODO
    1. I have to remove the udp-client project when I am done with tests.
    2. I have to encrypt or add a check sum to the samples.
    3. I have to provide install options and "install howto" in build section.
    4. Crypto project and lib for hashing or crypting is still a stub.
    5. GUI for the recorder is stub yet.
    6. I have to add Build/Install instructions and options to the BUILD section.
    7. Daemon logging must support sys/log functionality. Not done yet.
    8. GUI must perform Goertzl algorithm for DTM and filtering the sample data.

## BUGS
~~[Bug1]Strange bug as for 16.11.2016, when the program gives 100% cpu load on my other Fedora computer.~~
    [Fix Bug1] I've forgot a timer into server that has 10ms tick.

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
    08.11.2016: added new concept for swapping files when size or time reach limit.
    08.11.2016: refractory and good coding practices added.
    08.11.2016: started implementation of QWav based on Qt and QFile
    08.11.2016: implemented and tested QWav. Will use it in the future.
    10.11.2016: fixed a small bug for the size of files read from XML file
    11.11.2016: added udp error packet with samples filled to max
    11.11.2016: started implementing the gui context for displaying wav samples.
    11.11.2016: animation frame of the pointerof the data.
    11.11.2016: simple concepts for gui`s context.
    11.11.2016: changed the name of the executable binary.
    11.11.2016: added test plugin architecture.
    11.11.2016: added UID to file: <channel-UID-timestamp.wav>
    11.11.2016: plugin manager test: must pass the full path to .so
    12.11.2016: tested loading few test plugins to the application. OK.
    12.11.2016: now loading 2 different plugins.. had some name mangling problems. Solved.
    13.11.2016: tested on windows host machine, besides unix daemon everything works fine.
    13.11.2016: project cleanup and file organizing.
    14.11.2016: now can configure paths for logging and recording
    14.11.2016: now records wavs to a directory specified.
    14.11.2016: log writer will be configured for speed of logging.
    15.11.2016: fixed the bug with the hotswap, now testFileWatcher() is pending deprecation.
    15.11.2016: old hotswap logic moved to the timer based hotswap.
    15.11.2016: config support time based hotswaps and the size based.
    16.11.2016: Now we have a Logger class accessible form everywhere.
    16.11.2016: I have to test in lab env. the QWavWriter.
    16.11.2016: Signals to be handled by sockstreams via IPC.
    16.11.2016: Added more messages to logs.
    16.11.2016: Tested time based hotswap and configuration from xml.


***
<ilianzapryanov@abv.bg>

И.Z.
