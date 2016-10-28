#include "server.h"
#include "wav-writer.h"

// remove it after tests //
#include <iostream>

// defs //
#include "defs.h"

namespace iz {

RecConfig* Server::s_conf = new RecConfig("recorder.cfg");

Server::Server(QObject *parent)
    : QObject(parent),
      m_socket(nullptr),
      m_hearSocket(nullptr),
      m_logger("recorder.log"),
      m_senderHost("127.0.0.1"),
      m_senderPort(1234)
{
    // explicitly null all
    for(int i=0; i < 32; ++i) {
        m_wavs[i] = nullptr;
    }
    // pass the class to the static signal handlers
    m_logger.setObjectName("logger thread");
    m_logger.startWriter();

    m_heartbeat.setInterval(1000);
    connect(&m_heartbeat, SIGNAL(timeout()),
            this, SLOT(sendHeartbeat()));
}

Server::~Server()
{
    m_logger.stopWriter();
}

/// simple init function
/// \brief Server::init
/// \param is_daemon
/// \param udp maybe and tcp later
/// \param port
///
void Server::init(bool udp, quint16 port)
{

    for(int i=0; i < 32; ++i) {
        char fname[16]={0};
        sprintf(fname, "%d.wav", i+1);
        m_wavs[i] = new Wav(fname, s_conf);
        m_wavs[i]->open("wb");
    }
    if (udp) {
        m_socket = new QUdpSocket(this);
        m_hearSocket = new QUdpSocket(this);

        bool bres = m_socket->bind(port, QUdpSocket::ShareAddress);

        connect(m_socket, SIGNAL(readyRead()),
                this, SLOT(readyReadUdp())/*, Qt::DirectConnection*/);

        char msg[64]={0};
        if (bres) {
            std::cout << "Bind OK!" << std::endl;
            sprintf(msg,"Binding to port (%d) succeeds!\n", port);
            m_logger.write(msg);
            m_heartbeat.start();

        } else {
            std::cout << "Bind FAIL!" << std::endl;
            sprintf(msg,"Binding to port (%d) failed!\n", port);
            m_logger.write(msg);
        }
    } else {
        // TODO: implement the TCP logic
    }
    // starrt  logging writer //

}

/// ready read datagrams
/// \brief Server::readyReadUdp
///
void Server::readyReadUdp()
{
    while (m_socket->hasPendingDatagrams()) {

        QByteArray buff;

        buff.resize(m_socket->pendingDatagramSize());

        qint64 read = m_socket->readDatagram(buff.data(), buff.size(),
                               &m_senderHost, &m_senderPort);

        // just a test section to view the contents of the
        // packet
        wav_hdr_t* hdr = (wav_hdr_t*) buff.data();
        (void) hdr;
        // TODO
        if (read > 0) {
            // we have something
            // organize it and
            // send it to the writers

        } else {
            m_logger.write("Missed an UDP\n");
        }
    }
}

/// stub
/// \brief Server::readyReadTcp
///
void Server::readyReadTcp()
{
}

/// stub
/// \brief Server::handleConnection
///
void Server::handleConnection()
{
}

/// stub
/// \brief Server::route
///
void Server::route()
{
}

/// stub
/// \brief Server::sendHeartbeat
///
void Server::sendHeartbeat()
{
    m_hearSocket->writeDatagram("hearbeat", m_senderHost, m_senderPort);
}

void Server::writeToChannel(short data[], int len, int chan)
{
    if (chan < 0 || chan > 31) {
        return ;
    }
    m_wavs[chan]->write(data, len);
}

} // namespce iz
