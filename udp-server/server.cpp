// std //
#include <iostream> // for test purpose only!

// Daemon time //
#include <time.h> // for time stamping

#include "types.h"
#include "server.h"
#include "utils/logger.h"
#include "utils/wav-writer.h"
#include "unix/date-time.h"

namespace iz {

struct udp_data_t
{
    uint32_t    counter;
    uint8_t     null_bytes[32];
    int16_t    data[32][16];
    //int16_t    data[16][32];
};

struct tcp_data_t
{
    int16_t data[128];
};

// the err udp packet
static struct udp_data_t err_udp = {0, 0, 0};

Server::Server(QObject *parent)
    : QObject(parent),
      m_socket({nullptr}),
      m_hearSocket(nullptr),
      m_senderHost("127.0.0.1"), // default
      m_senderPort(1234),
      m_sendHeart(false)
{
}

Server::~Server()
{
}

/// simple init function
/// \brief Server::init
/// \param is_daemon
/// \param udp maybe and tcp later
/// \param port
///
void Server::init(bool udp, quint16 port, bool send_heart)
{
    Logger::Instance().logMessage("Initializing server...\n");
    // the error packet to be sent on packet lost
    for(int i=0; i < 32; ++i) {
        for(int j=0; j < 16; ++j) {
            err_udp.data[i][j] = 0x0000;
        }
    }

    m_sendHeart = send_heart;

    if (m_sendHeart) {
        m_heartbeat.setInterval(1000);
        connect(&m_heartbeat, SIGNAL(timeout()),
                this, SLOT(sendHeartbeat()));
    }
    if (udp) {
        m_socket.udp = new QUdpSocket(this);
        m_hearSocket = new QUdpSocket(this);

        bool bres = m_socket.udp->bind(port, QUdpSocket::ShareAddress);

        connect(m_socket.udp, SIGNAL(readyRead()),
                this, SLOT(readyReadUdp())/*, Qt::DirectConnection*/);

        char msg[64]={0};
        if (bres) {
            char msg2[128] ={0};
            sprintf(msg2, "Server started at: (%s)\n", DateTime::getDateTime());
            printf("Bind OK!\n");
            sprintf(msg,"Binding to port (%d) succeeds!\n", port);
            Logger::Instance().logMessage(msg);
            Logger::Instance().logMessage(msg2);
            // start timers

        } else {
            printf("Bind FAIL!\n");
            sprintf(msg,"Binding to port (%d) failed!\n", port);
            route(DISCONNECTED);
            Logger::Instance().logMessage(msg);
        }
    } else {
        m_socket.server = new QTcpServer(this);
        if (m_socket.server != nullptr)
        connect(m_socket.server, SIGNAL(newConnection()),
                this, SLOT(handleConnection()));

        if (!m_socket.server->listen(QHostAddress::Any, port)) {
            route(Server::DISCONNECTED);
        } else {
            route(Server::CONNECTED);
        }
    }
    // starrt  logging writer //
}

/// ready read datagrams
/// \brief Server::readyReadUdp
///
void Server::readyReadUdp()
{
    // write error udp to prevent wav size
    // fragmenation, if missed an udp,
    // I`ll write a 16 samples with max valuse

    if (m_socket.udp->hasPendingDatagrams()) {
        while (m_socket.udp->hasPendingDatagrams()) {

            QByteArray buff;

            buff.resize(m_socket.udp->pendingDatagramSize());

            qint64 read = m_socket.udp->readDatagram(buff.data(), buff.size(),
                                   &m_senderHost, &m_senderPort);


            // the udp structure from the device
            udp_data_t* udp = (udp_data_t*) buff.data();

            if (read > 0) {
                // packet loss logic below
                static uint32_t pktcnt = udp->counter;
                // one frame lost for synching with my counter

                if (udp->counter != ++pktcnt) {
                    static uint32_t lost_count = 0;
                    static char msg[128]={0};
                    sprintf(msg, "Packet lost:(%d) at: [%s]\tTotal lost:(%d)\n",
                            udp->counter, DateTime::getDateTime(),
                            lost_count);

                    lost_count++;

                    Logger::Instance().logMessage(msg);
                    pktcnt = udp->counter; // synch back

                    // always write a null bytes packet on missed udp

                    emit dataReady(err_udp);

                } else {
                // will use a new logic emit the udp struct
                // to the recorder, so now we don`t need
                // to depend each other
                    emit dataReady(*udp);
                }
             } else {
                Logger::Instance().logMessage("Missed an UDP!\n");
            }
        }
    } else {
        Logger::Instance()
                .logMessage("We can read, but there is no pending datagram!\n");
    }

}

/// Connect and read TCP packets
/// \brief Server::handleConnection
///
void Server::handleConnection()
{
    QTcpSocket* tcp = m_socket.server->nextPendingConnection();
    QByteArray bytes;
    if (tcp->canReadLine()) {
        bytes = tcp->readAll();
        tcp_data_t* data = (tcp_data_t*) bytes.data();
        emit dataReady(*data);
    }
}

/// dummy router for future uses of the states
/// \brief Server::route
///
void Server::route(States state)
{
    // handle state in this routing function
    // handle state in this routing function
    switch (state) {
    case DISCONNECTED:
        Logger::Instance().logMessage("Not connected!\n");
        break;  // try to reconnect
    case CONNECTED:
        Logger::Instance().logMessage("Connected!\n");
        break;
    case LOST_CONNECTION:
    case GOT_CONNECTION:
    case GOT_DATAGRAM:
    case MISSED_DATAGRAM:
    case UNKNOWN:
    default:
        break;
    }
}

/// stub
/// \brief Server::sendHeartbeat
///
void Server::sendHeartbeat()
{
    m_hearSocket->writeDatagram("hearbeat", m_senderHost, m_senderPort);
}

/// deinitialze the server, maybe some unfinished
/// task to be finalized here, or to be registered in
/// the daemon, will left it a TODO
/// \brief Server::deinit
///
void Server::deinit()
{
    // nothing for now
    Logger::Instance().logMessage("Deinitializing server...\n");
}

} // namespce iz
