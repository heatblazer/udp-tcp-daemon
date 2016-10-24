#include "server.h"
#include "wav-writer.h"


// remove it after tests //
#include <iostream>

namespace iz {

Server* Server::s_instance = nullptr;

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
void Server::init(bool udp, quint16 port)
{

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


void Server::route()
{

}

} // namespce iz
