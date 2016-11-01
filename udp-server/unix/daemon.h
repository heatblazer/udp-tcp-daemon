#ifndef DAEMON_H
#define DAEMON_H

// K&R //
#include <stdio.h>
#include <string.h>
#include <stdlib.h> // exit() and atoi()
#include <stdbool.h> // true/false

// unix //
#include <sys/stat.h> // chmod
#include <sys/mount.h> // mout fs
#include <sys/resource.h>
#include <signal.h>
#include <unistd.h> // cwd
#include <fcntl.h>
#include <errno.h>

namespace iz {
class SApplication;

typedef void (*sigHndl)(int, siginfo_t *,void*);

extern void daemonize();
extern void attachSignalHandler(sigHndl hnd, int slot);
extern void registerAppData(SApplication* data);

} // iz

#endif // DAEMON_H
