#include "server.h"
#include "wav-writer.h"

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

// remove it after tests //
#include <iostream>

namespace iz {

struct sigaction Server::m_signals[32];
Server* Server::s_instance = nullptr;

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
        fwrite(buff, 256, 256, fp);
        fflush(fp);
    }
    fclose(fp);
}



Server::Server(QObject *parent)
    : QObject(parent),
      m_socket({nullptr}),
      m_wavWriter("data.wav"),
      m_logger("log.log")
{
    // pass the class to the static signal handlers
    Server::s_instance = this;
}

Server::~Server()
{
    m_logger.stopWriter();
}

/// simple init function
/// \brief Server::init
/// \param is_daemon
/// \param udp
/// \param port
///
void Server::init(bool is_daemon, bool udp, quint16 port)
{

    if (is_daemon) {
        daemonize();
    }
    if (udp) {
        m_socket.udp = new QUdpSocket(this);
        m_socket.udp->bind(QHostAddress::LocalHost, port);
        connect(m_socket.udp, SIGNAL(readyRead()),
                this, SLOT(readyReadUdp()));

    } else {
        m_socket.tcp = new QTcpSocket(this);
        m_socket.tcp->bind(QHostAddress::LocalHost, port);
        connect(m_socket.tcp, SIGNAL(readyRead()),
                this, SLOT(readyReadTcp()));
    }
    // starrt  logging writer //
    m_logger.startWriter();

    // attach test sig handler to all sigs
    for(int i=1; i < 32; ++i) {
        attachSignalHandler(testSig, i);
    }
}

/// ready read datagrams
/// \brief Server::readyReadUdp
///
void Server::readyReadUdp()
{
    while (m_socket.udp->hasPendingDatagrams()) {
        QByteArray buff;
        buff.resize(m_socket.udp->pendingDatagramSize());
        QHostAddress sender;
        quint16 sender_port;

        m_socket.udp->readDatagram(buff.data(), buff.size(),
                               &sender, &sender_port);

        m_wavWriter.write(buff);
        if(/* something is not OK*/ 0) {
            m_logger.write("Some error message\n");
        }
    }
}

/// ready read tcp packets
/// \brief Server::readyReadTcp
///
void Server::readyReadTcp()
{
    while (m_socket.tcp->canReadLine()) {
        QByteArray buff = m_socket.tcp->readLine();
        m_wavWriter.write(buff);
        if(/* something is not OK*/ 0) {
            m_logger.write("Some error message\n");
        }
    }
}

void Server::daemonize()
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

void Server::attachSignalHandler(sigHndl hnd, int slot)
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

} // namespce iz
