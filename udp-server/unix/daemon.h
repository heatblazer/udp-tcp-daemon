#ifndef DAEMON_H
#define DAEMON_H

// unix signals //
#include <signal.h>

namespace iz {

// need a forward decl to pass to the register app foo()
class SApplication;

typedef void (*sigHndl)(int, siginfo_t *,void*);

extern void daemonize();
extern void attachSignalHandler(sigHndl hnd, int slot);
extern void registerAppData(SApplication* data);

} // iz

#endif // DAEMON_H
