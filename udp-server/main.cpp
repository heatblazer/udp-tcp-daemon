#include "sapplication.h"
#include "unix/daemon.h"
//#include "crypto.h"
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
               iz::daemonize();
               break;
           case 0:
           default:
               break;
           }
        }
    } else {
        std::cout << "ERROR!\n"
                  << "program -c <path to conf file>\n"
                  << "program --config <path to conf file>\n"
                  << std::endl;
        exit(4);
    }

    iz::SApplication app(argc, argv);
    int res = app.init();
    if (res < 0) {
        app.deinit();
        exit(4);
    }
    return app.exec();
}
