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
#include <signal.h>

// remove it after tests //
#include <iostream>

namespace iz {

Server::Server(QObject *parent)
    : QObject(parent),
      m_socket({nullptr}),
      m_wavWriter("data.wav"),
      m_logger("log.log")
{
    WavHdr h;
    h.bits_per_sample = 16;
    h.data[0] = 'd';
    h.data[0] = 'a';
    h.data[0] = 't';
    h.data[0] = 'a';
    h.file_size = 128;
    h.fmt[0] = 'r';
    h.fmt[1] = 'i';
    h.fmt[2] = 'f';
    h.fmt[3] = 'f';
    h.fsize = 12345678;

    m_test.writeHeader(&h);
}

Server::~Server()
{
    m_wavWriter.stopWriter();
    m_logger.stopWriter();
}

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
    // start writing to wav file //
    // TODO: test wit real wav stream
    // from client side
    m_wavWriter.startWriter();

    // starrt  logging writer //
    m_logger.startWriter();

}

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
    struct sigaction m_sig;
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

    m_sig.sa_handler = SIG_IGN;
    sigemptyset(&m_sig.sa_mask);
    m_sig.sa_flags = 0;

    if(sigaction(SIGHUP, &m_sig, NULL) < 0) {
        fprintf(stderr, "Can`t ignore SIGHUP\n");
        exit(3);
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

} // namespce iz
