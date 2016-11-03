#include "client.h"
#include <iostream>

#ifndef LOCAL_TEST
#define LOCAL_TEST
#endif

namespace iz {

Client::Client(QObject *parent)
    : QObject(parent)
{
    m_timer.setInterval(500);
    connect(&m_timer, SIGNAL(timeout()),
            this, SLOT(transmit()));
    m_addres("192.168.32.94");
}

Client::~Client()
{
}

void Client::init()
{
    p_socket = new QUdpSocket(this);
    p_socket->connectToHost(m_addres,
                            1234, QIODevice::WriteOnly);
    if (p_socket->state() == QUdpSocket::ConnectedState) {
        m_timer.start();
    }

}

void Client::transmit()
{
    std::cout << "Transmiting...\n";
    p_socket->write(m_packet.data, sizeof(m_packet.data)/sizeof(m_packet[0]));
}


} // iz
