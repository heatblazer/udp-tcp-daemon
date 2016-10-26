#ifndef SAPPLICATION_H
#define SAPPLICATION_H
// parent //
#include <QCoreApplication>

// K&R //
#include <stdio.h>
#include <string.h>
#include <stdlib.h> // exit() and atoi()
#include <stdbool.h> // true/false

// unix //
#include <sys/stat.h> // chmod
#include <sys/mount.h> // mout fs
#include <sys/resource.h>
#include <unistd.h> // cwd
#include <fcntl.h>
#include <errno.h>

// server //
#include "server.h"

// wav writer //
#include "wav-writer.h"

namespace iz {

typedef void (*sigHndl)(int, siginfo_t *,void*);

class SApplication : public QCoreApplication
{
public:
    explicit SApplication(int& argc, char** argv);
    virtual ~SApplication();
    static void daemonize();
    static void attachSignalHandler(sigHndl hnd, int slot);

    int init();

private:
    Server m_server;
    Wav*   m_wavs[32];
    QWav*  test_wav;
    static struct sigaction m_signals[32];

};

} // iz

#endif // SAPPLICATION_H
