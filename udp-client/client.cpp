#include "client.h"
#include <iostream>

// ansi C //
#include <math.h>
#include <time.h>


static double* sine_gen(int len)
{
    srand((unsigned)time(NULL));
    double* buff = (double*) malloc(len * (sizeof(double)));
    if (buff) {
        for(int i=0; i < len; ++i) {
            buff[i] = sin(2000 * (2 * M_PI) * i / 6000) + sqrt(0.01) * rand();
        }
        return buff;
    } else {
        return NULL;
    }
}

namespace iz {

Client::Client(QObject *parent)
    : QObject(parent),
      m_addres("127.0.0.1")
//      m_addres("192.168.32.94")
{
    m_timer.setInterval(500);
    connect(&m_timer, SIGNAL(timeout()),
            this, SLOT(transmit()));
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
    static uint32_t counter = 0;
    uint16_t* buff = (uint16_t*) sine_gen(32);
    if (buff) {
        std::cout << "Transmiting...\n";
        for(int i=0; i < 4; ++i) {
            memcpy(m_packet.packet.data[i], buff, 32);
        }
        memset(m_packet.packet.null_bytes, 0, sizeof(m_packet.packet.null_bytes)
                                                / sizeof(m_packet.packet.null_bytes[0]));

        m_packet.packet.counter = ++counter;
        p_socket->write(m_packet.data, sizeof(udp_data_t));
        free(buff);
    } else {
        std::cout << "No data.. \n";
    }
}


} // iz
