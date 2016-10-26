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

    FILE* fp = fopen("assets/1.wav", "rb");
    if (!fp) {
        std::cout << "Error opening wav file!" << std::endl;
    } else {
        fgets(m_data, 512, fp);
    }
    fclose(fp);
}

void Client::transmit()
{
    std::cout << "Transmiting...\n";

    p_socket->write(m_data);

}


} // iz
