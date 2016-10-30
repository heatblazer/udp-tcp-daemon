#include "daemon.h"

#include <fstream>

static std::ofstream    s_logFile("daemon.log", std::ofstream::out);
// all linux signals goes here
static struct sigaction s_signals[32];

namespace iz {

/// test sighandler
/// \brief testSig
/// \param a
/// \param info
/// \param usr_data
///
static void testSig(int a, siginfo_t *info ,void* usr_data)
{
    (void) usr_data;
    s_logFile << "args: " << a << std::endl;
    s_logFile.close();
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
    // get current dir
    getcwd(pwd, sizeof(pwd)/sizeof(pwd[0]));
    // set to current dir

    if (chdir(pwd) < 0) {
        s_logFile << "Could not set: ("
                  << pwd
                  << " )as cwd!\n" << std::endl;
        exit(3);
    } else {
        s_logFile << "Set ("
                  << pwd
                   <<  ") as cwd!" << std::endl;

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

        if(sigaction(SIGHUP, &s_signals[SIGHUP], NULL) < 0) {
            fprintf(stderr, "Can`t ignore SIGHUP\n");
            exit(3);
        } else {
            sigaction(slot, &s_signals[slot], NULL);
        }
    }
}

} // iz

// hinting
#if 0
/* Signals.  */
#define	SIGHUP		1	/* Hangup (POSIX).  */
#define	SIGINT		2	/* Interrupt (ANSI).  */
#define	SIGQUIT		3	/* Quit (POSIX).  */
#define	SIGILL		4	/* Illegal instruction (ANSI).  */
#define	SIGTRAP		5	/* Trace trap (POSIX).  */
#define	SIGABRT		6	/* Abort (ANSI).  */
#define	SIGIOT		6	/* IOT trap (4.2 BSD).  */
#define	SIGBUS		7	/* BUS error (4.2 BSD).  */
#define	SIGFPE		8	/* Floating-point exception (ANSI).  */
#define	SIGKILL		9	/* Kill, unblockable (POSIX).  */
#define	SIGUSR1		10	/* User-defined signal 1 (POSIX).  */
#define	SIGSEGV		11	/* Segmentation violation (ANSI).  */
#define	SIGUSR2		12	/* User-defined signal 2 (POSIX).  */
#define	SIGPIPE		13	/* Broken pipe (POSIX).  */
#define	SIGALRM		14	/* Alarm clock (POSIX).  */
#define	SIGTERM		15	/* Termination (ANSI).  */
#define	SIGSTKFLT	16	/* Stack fault.  */
#define	SIGCLD		SIGCHLD	/* Same as SIGCHLD (System V).  */
#define	SIGCHLD		17	/* Child status has changed (POSIX).  */
#define	SIGCONT		18	/* Continue (POSIX).  */
#define	SIGSTOP		19	/* Stop, unblockable (POSIX).  */
#define	SIGTSTP		20	/* Keyboard stop (POSIX).  */
#define	SIGTTIN		21	/* Background read from tty (POSIX).  */
#define	SIGTTOU		22	/* Background write to tty (POSIX).  */
#define	SIGURG		23	/* Urgent condition on socket (4.2 BSD).  */
#define	SIGXCPU		24	/* CPU limit exceeded (4.2 BSD).  */
#define	SIGXFSZ		25	/* File size limit exceeded (4.2 BSD).  */
#define	SIGVTALRM	26	/* Virtual alarm clock (4.2 BSD).  */
#define	SIGPROF		27	/* Profiling alarm clock (4.2 BSD).  */
#define	SIGWINCH	28	/* Window size change (4.3 BSD, Sun).  */
#define	SIGPOLL		SIGIO	/* Pollable event occurred (System V).  */
#define	SIGIO		29	/* I/O now possible (4.2 BSD).  */
#define	SIGPWR		30	/* Power failure restart (System V).  */
#define SIGSYS		31	/* Bad system call.  */
#define SIGUNUSED	31
#define	_NSIG		65	/* Biggest signal number + 1 */
#endif
