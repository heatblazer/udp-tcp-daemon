// remove it after tests //
#include <iostream>

// unix time //
#include <time.h>

#include "defs.h"
#include "server.h"
#include "wav-writer.h"

static uint8_t gen8bitCheckSum(char* data, int len)
{
    (void)gen8bitCheckSum;
    uint8_t key = 0;
    if (data == NULL) {
        return key;
    }
    for(int i=0; i < len; ++i) {
        key |= (1 << i) | (data[i] & 1);
    }
    return key;
}

static uint16_t gen16bitCheckSum(char* data, int len)
{
    (void)gen16bitCheckSum;
    uint16_t key = 0;
    if (data == NULL) {
        return key;
    }
    for(int i=0; i < len; ++i) {
        key |= (1 << i) | (data[i] & 1);
    }
    return key;
}

namespace iz {
#define UDP_SIZE (196)
union udp_data
{
    struct {
        int32_t counter;
        char null_bytes[32];
        char data[UDP_SIZE];
    } udp;
    char c[196];
};


RecConfig* Server::s_conf = new RecConfig("recorder.cfg");

Server::Server(QObject *parent)
    : QObject(parent),
      m_socket({nullptr}),
      m_hearSocket(nullptr),
      m_logger("recorder.log"),
      m_senderHost("127.0.0.1"),
      m_senderPort(1234)
{
    // explicitly null all
    for(int i=0; i < 32; ++i) {
        m_wavs[i] = nullptr;
    }
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

    for(int i=0; i < 32; ++i) {
        char fname[16]={0};
        sprintf(fname, "%d.wav", i+1);
        m_wavs[i] = new Wav(fname, s_conf);
        m_wavs[i]->open("wb");
    }
    if (udp) {
        m_socket.udp = new QUdpSocket(this);
        m_hearSocket = new QUdpSocket(this);

        bool bres = m_socket.udp->bind(port, QUdpSocket::ShareAddress);

        connect(m_socket.udp, SIGNAL(readyRead()),
                this, SLOT(readyReadUdp())/*, Qt::DirectConnection*/);

        char msg[64]={0};
        if (bres) {
            time_t current_time;
            struct tm * time_info;
            char timeString[9];  // space for "HH:MM:SS\0"

            time(&current_time);
            time_info = localtime(&current_time);

            strftime(timeString, sizeof(timeString), "%H:%M:%S", time_info);
            char msg2[64] ={0};
            sprintf(msg2, "Server started at: (%s)\n", timeString);

            std::cout << "Bind OK!" << std::endl;
            sprintf(msg,"Binding to port (%d) succeeds!\n", port);
            m_logger.write(msg);
            m_logger.write(msg2);
            m_heartbeat.start();

        } else {
            std::cout << "Bind FAIL!" << std::endl;
            sprintf(msg,"Binding to port (%d) failed!\n", port);
            m_logger.write(msg);
        }
    } else {
        m_socket.tcp = new QTcpSocket(this);
        m_socket.tcp->connectToHost(QHostAddress::LocalHost, port);
        connect(m_socket.tcp, SIGNAL(connected()),
                this, SLOT(handleConnection()));
    }
    // starrt  logging writer //

}

/// ready read datagrams
/// \brief Server::readyReadUdp
///
void Server::readyReadUdp()
{
    while (m_socket.udp->hasPendingDatagrams()) {

        QByteArray buff;

        buff.resize(m_socket.udp->pendingDatagramSize());

        qint64 read = m_socket.udp->readDatagram(buff.data(), buff.size(),
                               &m_senderHost, &m_senderPort);

        // just a test section to view the contents of the
        // packet

        // counter!!!
        int32_t* b = (int32_t*)buff.data();
        static QByteArray pcm;

        if (read > 0) {
            static int32_t pktcnt = *b;

            // one time lost for synching
            if (*b != ++pktcnt) {
                time_t current_time;
                struct tm * time_info;
                char timeString[9];  // space for "HH:MM:SS\0"

                time(&current_time);
                time_info = localtime(&current_time);

                strftime(timeString, sizeof(timeString), "%H:%M:%S", time_info);

                char msg[64]={0};
                sprintf(msg, "Packet lost:(%d) at: [%s]\n", *b, timeString);

                m_logger.write(QByteArray(msg));
                pktcnt = *b;
            }
////////////////// remove after test //////////////////
            // we have something
            // organize it and
            // send it to the writers
                static bool write = true;
            static int cnt = 0;
            if (cnt++ > 200 && write) {
                write = false;
                m_wavs[4]->write((short*)pcm.data(), pcm.size());
                m_wavs[4]->close();
            }
            if (write) {
                pcm.append((char*)b, 196);
            }

         } else {
            std::cout << "Missed an UDP" << std::endl;
            m_logger.write("Missed an UDP\n");
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
    while (m_socket.tcp->canReadLine()) {
        // do something here
    }
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
#ifndef TEST
#else
    m_hearSocket->writeDatagram("hearbeat", m_senderHost, m_senderPort);
#endif
}

void Server::writeToChannel(short data[], int len, int chan)
{
    if (chan < 0 || chan > 31) {
        return ;
    }
    m_wavs[chan]->write(data, len);
}



} // namespce iz
