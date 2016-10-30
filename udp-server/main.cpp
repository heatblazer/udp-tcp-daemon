#include "sapplication.h"
#include "daemon.h"

#define WAV_TEST 1


#ifdef WAV_TEST
int main(int argc, char *argv[])
{
    (void) argc;
    (void) argv;
    // configure is buggy parameters
    iz::RecConfig conf("recorder.cfg");
    iz::Wav w("test.wav", &conf);
    w.open("wb");
    short garbage[64*1024]; // not initialzed on purpose
    w.write(garbage, 64 * 1024);
    w.close();
    return 0;
}
#else
/// for now one option for
/// daemonizing
/// \brief getOpts
/// \param str
/// \return
///
static int getOpts(char* str)
{
    int opts = 0;
    if ( (strcmp(str, "-d") == 0) ||
         (strcmp(str, "--daemon") == 0)) {
        opts = 1;
    } else {
        opts = 0;
    }
    return opts;
}

int main(int argc, char *argv[])
{
    // if no args - run in lab mode to test
    // and debug , later we`ll be sure
    // the >1 arg is provided
    if (argc > 1) {
        int opts = 0;
        for(int i=0; i < argc; ++i) {
           opts = getOpts(argv[i]);
           switch(opts) {
           case 1: // and only for now
               iz::daemonize();
               break;
           case 0:
           default:
               break;
           }
        }
    }

    iz::SApplication app(argc, argv);
    app.init();
    return app.exec();
}
#endif
