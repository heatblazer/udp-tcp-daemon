#include "server.h"
#include "wav-writer.h"

// remove it after tests //
#include <iostream>

namespace iz {

Server::Server(QObject *parent)
    : QObject(parent),
      m_socket(nullptr),
      m_logger("log.log")
{
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

    if (udp) {
        m_socket = new QUdpSocket(this);

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
    std::cout << "Ready read UDP!" << std::endl;

    while (m_socket->hasPendingDatagrams()) {

        QByteArray buff;
        buff.resize(m_socket->pendingDatagramSize());
        QHostAddress sender;
        quint16 sender_port;

        qint64 read = m_socket->readDatagram(buff.data(), buff.size(),
                               &sender, &sender_port);
        if (read > 0) {
            // we have something
            // organize it and
            // send it to the writers
        }

        if(/* something is not OK*/ 0) {
            m_logger.write("Some error message\n");
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
    // send instant datagram to host
    // each 1 second to eport we are alive
}

} // namespce iz
