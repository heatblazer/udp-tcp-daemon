#include "server.h"
#include "wav-writer.h"


// remove it after tests //
#include <iostream>

namespace iz {

Server* Server::s_instance = nullptr;

Server::Server(QObject *parent)
    : QObject(parent),
      m_socket(nullptr),
      m_inaddr(nullptr),
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
        m_socket = new QUdpSocket(this);

        //if (m_socket->open(QIODevice::ReadOnly)) {
        //    std::cout << "Opened socked!" << std::endl;
        //}

        bool bres = m_socket->bind(port, QUdpSocket::ShareAddress);

        connect(m_socket, SIGNAL(readyRead()),
                this, SLOT(readyReadUdp())/*, Qt::DirectConnection*/);

        if (bres) {
            std::cout << "Bind OK!" << std::endl;
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

    while (m_inaddr->hasPendingDatagrams()) {

        QByteArray buff;
        buff.resize(m_inaddr->pendingDatagramSize());
        QHostAddress sender;
        quint16 sender_port;

        qint64 read = m_inaddr->readDatagram(buff.data(), buff.size(),
                               &sender, &sender_port);
        if (read > 0) {
            m_wavWriter.write(buff);
        }
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
#if 0
    while (m_socket.tcp->canReadLine()) {
        QByteArray buff = m_socket.tcp->readLine();
        m_wavWriter.write(buff);
        if(/* something is not OK*/ 0) {
            m_logger.write("Some error message\n");
        }
    }
#endif
}

void Server::handleConnection()
{
    std::cout << "Connected to host" << std::endl;
    m_inaddr = new QUdpSocket(this);
    connect(m_inaddr, SIGNAL(readyRead()),
            this, SLOT(readyReadUdp()));

    QHostAddress addr = m_socket->peerAddress();
    quint16 port = m_socket->peerPort();

    bool conn = m_inaddr->bind(1234);
    if (conn) {
        //m_inaddr->connectToHost(QHostAddress("192.168.32.94"), 1234);
        std::cout << "Bound to: " << addr.toString().toStdString()
                  << " port:"
                  << port << std::endl;
    }

}


void Server::route()
{

}

} // namespce iz
