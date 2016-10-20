#include "client.h"

namespace iz {

Client::Client(QObject *parent)
    : QObject(parent)
{
}

Client::~Client()
{
    delete p_socket;
}

void Client::init()
{
    p_socket = new QUdpSocket(this);
}

void Client::transmit()
{
    static int test = 0;
    while (test++ < 100000) {
        QByteArray data("test data");
        p_socket->writeDatagram(data, QHostAddress::LocalHost, 7755);
    }
}


} // iz
