// unix time //
#include <time.h> // for time stamping

#include "server.h"
#include "wav-writer.h"


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


/// generate small unsigned checksum
/// based on the LSB of a data stream
/// with a fixed length
/// \brief gen8bitCheckSum
/// \param data
/// \param len
/// \return
///
static uint8_t gen8bitCheckSum(char* data, int len=8)
{
    (void)gen8bitCheckSum;
    uint8_t key = 0;
    if (data == NULL) {
        return key;
    }
    for(int i=0; i < len; ++i) {
        key |= ((data[i] & 0x1) << i);
    }
    return key;
}

/// generate a bigger chechksum
/// based on the LSB of the data stream
/// with a fixed length
/// \brief gen16bitCheckSum
/// \param data
/// \param len
/// \return
///
static uint16_t gen16bitCheckSum(char* data, int len=16)
{
    (void)gen16bitCheckSum;
    uint16_t key = 0;
    if (data == NULL) {
        return key;
    }
    for(int i=0; i < len; ++i) {
        key |= ((data[i] & 0x1) << i);
    }
    return key;
}

namespace iz {

struct udp_data
{
    uint32_t    counter;
    uint8_t     null_bytes[32];
    uint16_t    data[80];
};


RecConfig* Server::s_conf = new RecConfig("recorder.cfg");

Server::Server(QObject *parent)
    : QObject(parent),
      m_socket({nullptr}),
      m_hearSocket(nullptr),
      m_logger("recorder.log"),
      m_senderHost("127.0.0.1"), // default
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
    // now I have a pretty sine wave
    m_recTime.setInterval(10000); // 10 seconds for test recording
                                    // closes program - crashes for test

    connect(&m_heartbeat, SIGNAL(timeout()),
            this, SLOT(sendHeartbeat()));
    connect(&m_recTime, SIGNAL(timeout()),
            this, SLOT(stopRecording()));
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
        udp_data* udp = (udp_data*) buff.data();

        if (read > 0) {
            // packet loss logic below
            // TODO: check if the counter is signed or unsigned
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
            // we can now write data to channels ...
            // write data in this section
            // organize bytes and write them to the files
            // TODO:
            m_wavs[0]->write((short*)udp->data, 80);

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

/// handle the timeout by stop
/// recording
/// \brief Server::stopRecording
///
void Server::stopRecording()
{
    for (int i=0; i < 32; ++i) {
        m_wavs[i]->close();
    }

#ifdef TEST
    exit(0);
#endif
}

} // namespce iz
