#include "client.h"
#include <iostream>

#ifndef LOCAL_TEST
#define LOCAL_TEST
#endif

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
#ifdef LOCAL_TEST

#else
    p_socket->connectToHost(QHostAddress("192.168.32.154"),
                            1234, QIODevice::WriteOnly);
#endif


    FILE* fp = fopen("assets/1.wav", "rb");
    if (!fp) {
        std::cout << "Error opening wav file!" << std::endl;
    } else {
        memset(m_data, 0, sizeof(m_data));
        char hdr[256]={0};
        fseek(fp, 0U, SEEK_SET);
        fread(hdr, sizeof(hdr), 1,fp);
        char* c = &hdr[44];
        for(int i=0; i < 196; ++i) {
            m_data[i] = c[i];
        }
        int i = 0xdeadcafe;

    }
    fclose(fp);
    m_timer.start();

}

void Client::transmit()
{
    std::cout << "Transmiting...\n";
    p_socket->writeDatagram(m_data, QHostAddress::LocalHost, 1234);

}


} // iz
