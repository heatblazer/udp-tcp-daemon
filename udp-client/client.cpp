#include "client.h"
#include <iostream>
namespace iz {

Client::Client(QObject *parent)
    : QObject(parent)
{
    m_timer.setInterval(2000);
    connect(&m_timer, SIGNAL(timeout()),
            this, SLOT(transmit()));
}

Client::~Client()
{
}

void Client::init()
{
    p_socket = new QUdpSocket(this);
    p_socket->connectToHost(QHostAddress("192.168.32.154"),
                            1234, QIODevice::WriteOnly);
    if (p_socket->state() == QUdpSocket::ConnectedState) {
        m_timer.start();
    }
}

void Client::transmit()
{
    std::cout << "Transmiting...\n";
    QByteArray data("test data");
    p_socket->write(data);

}


} // iz
