#include "server.h"
#include "wav-writer.h"

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
    m_wavWriter.startWriter();
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

} // namespce iz
