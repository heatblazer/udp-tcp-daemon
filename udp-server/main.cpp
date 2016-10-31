#include "sapplication.h"
#include "daemon.h"
#include "crypto.h"


#ifdef WAV_TEST
int main(int argc, char *argv[])
{
    (void) argc;
    (void) argv;
    // configure is buggy parameters
    iz::RecConfig conf("recorder.cfg");
    iz::Wav w("test.wav", &conf);
    w.open("wb");
    char garbage[64*1024]; // not initialzed on purpose
    w.write((short*)garbage, 64 * 1024);
    w.close();

    return 0;
}
#elif defined (CRYPTO_TEST)
// I will test crypt algorithms here
// and checksums for SHA and MD5 stuff here
int main(int argc, char *argv[])
{
    (void) argc;
    (void) argv;
    Crypto c;
    c.dummy();
    return 0;
}
#else
// the standart server test - passed for now
// I got a correct sine wave from the client
// and recorded it to a wav file
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
    iz::registerAppData(&app);
    app.init();
    return app.exec();
}
#endif
