// unix time //
#include <time.h> // for time stamping

#include "types.h"
#include "server.h"
#include "utils/wav-writer.h"

/// timestring
/// \brief getTimeString
/// \return
///
static inline char* getTimeString()
{
    (void) getTimeString;
    time_t current_time;
    struct tm * time_info;
    static char timeString[9];  // space for "HH:MM:SS\0"

    time(&current_time);
    time_info = localtime(&current_time);
    strftime(timeString, sizeof(timeString), "%H:%M:%S", time_info);
    return timeString;
}

namespace iz {

Server::Server(QObject *parent)
    : QObject(parent),
      m_socket({nullptr}),
      m_hearSocket(nullptr),
      // to be configured!
      m_logger(QString("%1-recorder.log").arg(getTimeString())),
      m_senderHost("127.0.0.1"), // default
      m_senderPort(1234)
{

    // pass the class to the static signal handlers
    m_logger.setObjectName("logger thread");
    m_logger.startWriter();

    m_heartbeat.setInterval(1000);
    // now I have a pretty sine wave
    m_recTime.setInterval(10000); // 10 seconds for test recording
                                    // closes program - crashes for test

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
    if (udp) {
        m_socket.udp = new QUdpSocket(this);
        m_hearSocket = new QUdpSocket(this);

        bool bres = m_socket.udp->bind(port, QUdpSocket::ShareAddress);

        connect(m_socket.udp, SIGNAL(readyRead()),
                this, SLOT(readyReadUdp())/*, Qt::DirectConnection*/);

        char msg[64]={0};
        if (bres) {
            char msg2[64] ={0};
            sprintf(msg2, "Server started at: (%s)\n", getTimeString());
            printf("Bind OK!\n");

            sprintf(msg,"Binding to port (%d) succeeds!\n", port);
            route(CONNECTED);
            m_logger.write(msg);
            m_logger.write(msg2);
            // start timers
            m_heartbeat.start();
            m_recTime.start();

        } else {
            printf("Bind FAIL!\n");
            sprintf(msg,"Binding to port (%d) failed!\n", port);
            route(DISCONNECTED);
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


        // the udp structure from the device
        udp_data_t* udp = (udp_data_t*) buff.data();

        if (read > 0) {
            // packet loss logic below
            static uint32_t pktcnt = udp->counter;
            // one frame lost for synching with my counter
            if (udp->counter != ++pktcnt) {
                time_t current_time;
                struct tm * time_info;
                char timeString[9];  // space for "HH:MM:SS\0"

                time(&current_time);
                time_info = localtime(&current_time);

                strftime(timeString, sizeof(timeString), "%H:%M:%S", time_info);

                char msg[64]={0};
                sprintf(msg, "Packet lost:(%d) at: [%s]\n",
                        udp->counter, timeString);

                m_logger.write(QByteArray(msg));
                pktcnt = udp->counter; // synch back
            }

            // will use a new logic emit the udp struct
            // to the recorder, so now we don`t need
            // to depend each other
            emit dataReady(*udp, /*desired slot*/ 0);

         } else {
            printf("Missed an UDP\n");
            m_logger.write("Missed an UDP\n");
        }
    }
}

/// handle incomming TCP connections
/// \brief Server::readyReadTcp
///
void Server::readyReadTcp()
{
}

/// Connect and read TCP packets
/// \brief Server::handleConnection
///
void Server::handleConnection()
{
    if (m_socket.tcp->canReadLine()) {
        while (m_socket.tcp->canReadLine()) {
            QByteArray data = m_socket.tcp->readLine();
            (void) data;
        }
    }
}

/// dummy router for future uses of the states
/// \brief Server::route
///
void Server::route(States state)
{
    // handle state in this routing function
    switch (state) {
    case DISCONNECTED:
    case CONNECTED:
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
#ifdef TEST
#else
    m_hearSocket->writeDatagram("hearbeat", m_senderHost, m_senderPort);
#endif
}

void Server::deinit()
{
    // nothing for now
}

} // namespce iz
