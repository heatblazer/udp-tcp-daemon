#ifdef QWAVE
#include <time.h>
#include <math.h>
#include "utils/qwave-writer.h"

static double* sine_gen(int len)
{
    srand((unsigned)time(NULL));
    double* buff = (double*) malloc(len * (sizeof(double)));
    if (buff) {
        for(int i=0; i < len; ++i) {
            buff[i] = sin(2000 * (2 * M_PI) * i / 6000) + sqrt(0.01) * rand();
        }
        return buff;
    } else {
        return NULL;
    }
}

int main(int argc, char** argv)
{
    (void) argc;
    (void) argv;
    iz::QWav wav("test.wav");
    wav.setupWave();
    wav.open(iz::QWav::WriteOnly, 10);
    double* sine = sine_gen(64333);

    for (int i=0; i < 10; ++i) {
        wav.write((short*) sine, 64333);
    }
    wav.close();

    return 0;
}
#elif PLUGIN_TEST

#include "plugin-manager.h"


int main(int argc, char** argv)
{
    if (argc < 2) {
        return 1;
    }

    bool res = iz::RecPluginMngr::loadLibrary(QString(argv[1]));
    if (res) {
        iz::RecIface *dummy =
                iz::RecPluginMngr::getInterface(QString(argv[1]));
        if (dummy) {
            dummy->init();
            dummy->get_data();
            dummy->put_data(0);
            dummy->put_ndata(0, 0);
        }
    }

    return 0;
}

#else
#include "sapplication.h"
#include "unix/daemon.h"
#include "utils/recorder-config.h"
#include "types.h"
#include <iostream>

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
               iz::Daemon::daemonize();
               break;
           case 0:
           default:
               break;
           }
        }
    } else {
        std::cout << "Usage:\n"
                  << "Load config: recd2 -c <path to conf file>\n"
                  << "Load config: recd2 --config <path to conf file>\n"
                  << "Help: recd2 -h\n"
                  << "Help: recd2 --help\n"
                  << "Daemonize: recd2 -d\n"
                  << std::endl;
        std::cout << "You will be entering a failsafe mode with defaults."
                  << std::endl;
    }

    iz::SApplication app(argc, argv);
    int res = app.init();
    if (res < 0) {
        std::cout << "Error!\n"
                  << "Program failed to initalize with error: ("
                  << res << ") " << std::endl;
        exit(4);
    }
    return app.exec();
}
#endif
