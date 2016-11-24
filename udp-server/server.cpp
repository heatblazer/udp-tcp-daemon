// qt //
#include <QNetworkReply>

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
      m_sendHeart(false),
      m_conn_info({0, 0, 0, false})
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
            err_udp.data[i][j] = 37222;
        }
    }

    m_sendHeart = send_heart;

    if (m_sendHeart) {
        // heartattack for flooding for testing
        m_heartbeat.setInterval(10);
        connect(&m_heartbeat, SIGNAL(timeout()),
                this, SLOT(sendHeartbeat()));
    }
    if (udp) {
        m_socket.udp = new QUdpSocket(this);
        m_hearSocket = new QUdpSocket(this);

        bool bres = m_socket.udp->bind(port, QUdpSocket::ShareAddress);

        connect(m_socket.udp, SIGNAL(readyRead()),
                this, SLOT(readyReadUdp())/*, Qt::DirectConnection*/);

        connect(m_socket.udp, SIGNAL(disconnected()),
                this, SLOT(disconnected()));
        connect(m_socket.udp, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
                this, SLOT(hStateChange(QAbstractSocket::SocketState)));
        connect(m_socket.udp, SIGNAL(error(QAbstractSocket::SocketError)),
                this, SLOT(error(QAbstractSocket::SocketError)));


        char msg[64]={0};
        if (bres) {
            char msg2[128] ={0};
            snprintf(msg2, sizeof(msg2),
                     "Server started at: (%s)\n"
                     "Monitor live each 1000 ms!\n", DateTime::getDateTime());

            printf("Bind OK!\n");
            snprintf(msg, sizeof(msg), "Binding to port (%d) succeeds!\n", port);
            Logger::Instance().logMessage(msg);
            Logger::Instance().logMessage(msg2);
            // start timers
            m_liveConnection.setInterval(1000);
            connect(&m_liveConnection, SIGNAL(timeout()),
                    this, SLOT(checkConnection()));
            m_liveConnection.start();

        } else {
            printf("Bind FAIL!\n");
            snprintf(msg, sizeof(msg), "Binding to port (%d) failed!\n", port);
            route(DISCONNECTED);
            Logger::Instance().logMessage(msg);
        }
    } else {
        // unused tcp logic for now
    }
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
////////////////// deleteme later ///////////////////
#ifdef HEARTATTACK
            static bool onetime = false;
            if (!onetime) {
                onetime = true;
                m_heartbeat.start();
            }
#endif
            if (read > 0) {
                // the udp structure from the device
                udp_data_t* udp = (udp_data_t*) buff.data();

                // one frame lost for synching with my counter

                if (udp->counter != ++m_conn_info.paketCounter) {
                    static char msg[300] = {0};

                    snprintf(msg, sizeof(msg),
                             "Last synch packet:(%d)\t at: [%s]\n"
                             "Total desynch:(%d)\n"
                             "Server counter: (%d)\n"
                             "Lost: (%d)\n"
                             "Total lost: (%d)\n",
                             udp->counter, // next got ocunter
                             DateTime::getDateTime(),       // current time
                             m_conn_info.desynchCounter,    // desync counter
                             m_conn_info.paketCounter,      // server counter
                             (udp->counter - m_conn_info.paketCounter),  // lost
                             m_conn_info.totalLost);               // total lost

                    m_conn_info.desynchCounter++;
                    int errs = udp->counter - m_conn_info.paketCounter;
                    m_conn_info.totalLost += errs;

                    Logger::Instance().logMessage(msg);
                    m_conn_info.paketCounter = udp->counter; // synch back

                    // always write a null bytes packet on missed udp
                    if(!m_conn_info.onetimeSynch) {
                        m_conn_info.onetimeSynch = true;
                        emit dataReady(err_udp);
                    } else {
                        for(int i=0; i < errs; ++i) {
                            emit dataReady(err_udp);
                        }
                    }

                } else {
                // will use a new logic emit the udp struct
                // to the recorder, so now we don`t need
                // to depend each other
                    m_monitorData.append(*udp);
                    emit dataReady(*udp);
                }
             } else {
                Logger::Instance().logMessage("Missed an UDP!\n");
            }
        }
    } else {
        Logger::Instance()
                .logMessage("We can read, but there is no pending datagram!\n");
        disconnected();
    }
}

/// Connect and read TCP packets
/// \brief Server::handleConnection
///
void Server::handleConnection()
{
    // unimplemented
}

void Server::hStateChange(QAbstractSocket::SocketState state)
{
    (void) state;
}

void Server::error(QAbstractSocket::SocketError err)
{
    std::cout << "Got err: "  << err << std::endl;
}

/// monitor of the incomming datagrams
/// \brief Server::checkConnection
/// check periodically for datagrams coming from
/// outside.
void Server::checkConnection()
{
    if (m_monitorData.isEmpty()) {
        // ok
        std::cout << "NOT OK!\n";
        disconnected();
    } else {
        // not ok!
        //disconnected();
        std::cout << "OK\n";
        // make sure you purge the list
        if (m_monitorData.count() > 100) {
            m_monitorData.clear();
        }
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
        disconnected();
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

void Server::disconnected()
{
    static char msg[350] = {0};
    snprintf(msg, sizeof(msg), "Lost connection!\n"
             "Last desynch counter: (%d)\n"
             "Last total lost packages: (%d)\n"
             "Last pakcet counter: (%d)\n",
             m_conn_info.desynchCounter,
             m_conn_info.totalLost,
             m_conn_info.paketCounter);

    Logger::Instance().logMessage(msg);
    m_conn_info.desynchCounter = 0;
    m_conn_info.paketCounter = 0;
    m_conn_info.totalLost = 0;
    m_conn_info.onetimeSynch = false;
}

/// stub
/// \brief Server::sendHeartbeat
///
void Server::sendHeartbeat()
{
    static const char* heart_attack =
#ifdef HEARTATTACK
    "..................................................."
    "..................................................."
    "..................................................."
    "..................................................."
    "..................................................."
#else
    "heartbeat"
#endif
    ;

    m_hearSocket->writeDatagram(heart_attack, m_senderHost, m_senderPort);
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

ServerThread::ServerThread(QThread *parent)
    : QThread(parent),
      p_usr(nullptr)
{
}

ServerThread::~ServerThread()
{
}

void ServerThread::run()
{
    p_usr = new UserServer;
    p_usr->startServer();
    exec();
}

UserServer::UserServer(QObject *parent)
    : QTcpServer(parent),
      p_conn(nullptr)
{
}

UserServer::~UserServer()
{

}

/// starts user`s server
/// \brief UserServer::startServer
///
void UserServer::startServer()
{
    int port = 5678;
    connect(this, SIGNAL(newConnection()),
            this, SLOT(hConnection()));

    if (!this->listen(QHostAddress::Any, port)) {
        std::cout << "Could not start user server\n";

    } else {
        std::cout << "Started user server!\n";
    }
}

/// prints a base menu to the user
/// \brief UserServer::hConnection
///
void UserServer::hConnection()
{
    while (p_conn->canReadLine()) {
        QByteArray line = p_conn->readLine();
        // echo back
        QByteArray resp;
        if (line.contains("help")) {
            resp.append("Help unavailable yet. See you later!\n");
        } else if (line.contains("version")) {
            resp.append("Version... \n");
        } else if (line.contains("quit")) {
            resp.append("BYE!\n");
            p_conn->write(resp);
            p_conn->flush();
            p_conn->waitForBytesWritten();
            p_conn->disconnectFromHost();
            return;
        } else {
            resp.append("Unknown command!\n"
                        "Refer to: 'help', 'version' or 'quit' for now!\n");
        }
        p_conn->write(resp);
        p_conn->flush();
        p_conn->waitForBytesWritten();

    }
}

void UserServer::disconnected()
{
    p_conn->deleteLater();
}

/// TODO: open socket and respond back to user
/// \brief UserServer::incomingConnection
/// \param socketDescriptor
///
void UserServer::incomingConnection(qintptr socketDescriptor)
{
    std::cout << "Connection coming from: (" << socketDescriptor << ")" << std::endl;
    p_conn = new QTcpSocket;
    if (!p_conn->setSocketDescriptor(socketDescriptor)) {
        std::cout << "Can`t setup user response!\n";
    } else {
        connect(p_conn, SIGNAL(readyRead()),
                this, SLOT(hConnection()), Qt::DirectConnection);
        connect(p_conn, SIGNAL(disconnected()),
                this, SLOT(disconnected()));
    }
}

} // namespce iz
