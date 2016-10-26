#include "daemon.h"

namespace iz {

static struct sigaction s_signals[32];

/// test sighandler
/// \brief testSig
/// \param a
/// \param info
/// \param usr_data
///
static void testSig(int a, siginfo_t *info ,void* usr_data)
{
    (void) usr_data;
    FILE* fp = fopen("siglog.log", "a+");
    if(!fp) {
        return;
    } else {
        char buff[256]={0};
        sprintf(buff, "args: [%d]\tinfo: signo:[%d]\t\n",
                a, info->si_signo);
        fwrite(buff, sizeof(buff), strlen(buff), fp);
        fflush(fp);
    }
    fclose(fp);
    exit(10); // test exit
}



void daemonize()
{
    umask(0);
    struct rlimit rl;
    pid_t pid;

    if (getrlimit(RLIMIT_NOFILE, &rl) < 0) {
        fprintf(stderr, "Error: (%d) : (%s)\n", errno,
                strerror(errno));
        exit(3);
    }

    if (getppid() == 1) {
        // already deamon
        return;
    }
    pid = fork();
    if (pid < 0) {
        fprintf(stderr, "Fork failed: (%d) : (%s)\n", errno,
                strerror(errno));
        exit(1);
    } else if (pid > 0) {
        fprintf(stdout, "Parent exists: (%d) : (%s)\n", errno,
                strerror(errno));
        exit(0);
    } else {
        //fprintf(stdout, "Forked a daemon process!\n");
        //exit(0);
    }


    if (setsid() == -1) {
        exit(EXIT_FAILURE);
    }

    for(int i=0; i < 32; ++i) {
        s_signals[i].sa_sigaction = NULL;
        sigemptyset(&s_signals[i].sa_mask);
        s_signals[i].sa_flags = 0;
    }

    if ((pid = fork()) < 0) {
        exit(3);
    } else if (pid > 0) {
        exit(0);
    }

    char pwd[512]={0};
    getcwd(pwd, sizeof(pwd)/sizeof(pwd[0]));

    if (chdir(pwd) < 0) {
        exit(3);
    }

    if (rl.rlim_max == RLIM_INFINITY) {
        rl.rlim_max = 1024;
    }

    for(unsigned i=0; i < rl.rlim_max; ++i) {
        close(i);
    }

    int fd0, fd1, fd2;
    fd0 = open("/dev/null", O_RDWR);
    fd1 = dup(0);
    fd2 = dup(0);
    // why does ide says they are unused???
    (void) fd0; (void) fd1; (void) fd2;

    for(int i=0; i < 32; ++i) {
        attachSignalHandler(&testSig, i);
    }
}

/// for future use:
/// attach handlers to specific signals
/// \brief attachSignalHandler
/// \param hnd
/// \param slot
///
void attachSignalHandler(sigHndl hnd, int slot)
{
    if (slot < 1 || slot > 31) {
        // out of index signal mapping
        return;
    } else {
        s_signals[slot].sa_sigaction = hnd;
        // union - set a handler function!

        if(sigaction(SIGHUP, &s_signals[SIGHUP], NULL) < 0) {
            fprintf(stderr, "Can`t ignore SIGHUP\n");
            exit(3);
        } else {
            sigaction(slot, &s_signals[slot], NULL);
        }
    }
}



} // iz
