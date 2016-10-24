#include "sapplication.h"

namespace iz {

struct sigaction SApplication::m_signals[32];

// usr_data can be cast to the object that
// called the sighandler... not useful
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
}

/// TODO: add arguments to the program
/// \brief SApplication::SApplication
/// \param argc
/// \param argv
///
SApplication::SApplication(int &argc, char **argv)
    : QCoreApplication(argc, argv)
{
}

SApplication::~SApplication()
{
}

int SApplication::init(bool is_daemon)
{
    // attach test sig handler to all sigs
    if (is_daemon) {
        for(int i=1; i < 32; ++i) {
            attachSignalHandler(testSig, i);
        }
        daemonize();
    }
    m_server.init();
    return 1;
}

void SApplication::daemonize()
{
    umask(0);
    struct rlimit rl;
    pid_t m_pid;

    if (getrlimit(RLIMIT_NOFILE, &rl) < 0) {
        fprintf(stderr, "Error: (%d) : (%s)\n", errno,
                strerror(errno));
        exit(3);
    }

    m_pid = fork();

    if (m_pid < 0) {
        fprintf(stderr, "Fork failed: (%d) : (%s)\n", errno,
                strerror(errno));
        exit(3);
    } else  if (m_pid != 0){
        exit(EXIT_SUCCESS);
    }

    if (setsid() == -1) {
        exit(EXIT_FAILURE);
    }

    for(int i=0; i < 32; ++i) {
        m_signals[i].sa_sigaction = NULL;
        sigemptyset(&m_signals[i].sa_mask);
        m_signals[i].sa_flags = 0;
    }

    if ((m_pid = fork()) < 0) {
        exit(3);
    } else if (m_pid > 0) {
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

}

void SApplication::attachSignalHandler(sigHndl hnd, int slot)
{
    if (slot < 1 || slot > 31) {
        return;
    } else {
        m_signals[slot].sa_sigaction = hnd;
        // union - set a handler function!

        if(sigaction(SIGHUP, &m_signals[SIGHUP], NULL) < 0) {
            fprintf(stderr, "Can`t ignore SIGHUP\n");
            exit(3);
        } else {
            sigaction(slot, &m_signals[slot], NULL);
        }
    }
}

} // iz
