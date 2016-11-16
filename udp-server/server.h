#ifndef SERVER_H
#define SERVER_H

// qt //
#include <QFile>
#include <QObject>
#include <QTimer>
#include <QTcpSocket>
#include <QTcpServer>
#include <QUdpSocket>

// custom //
#include "types.h"
#include "utils/writer.h"
#include "utils/wav-writer.h"

namespace iz {

class Server : public QObject
{
    Q_OBJECT
public:
    // for future uses
    enum States {
                UNKNOWN=0,
                DISCONNECTED = 1,
                CONNECTED = 2,
                LOST_CONNECTION = 3,
                GOT_CONNECTION = 4,
                GOT_DATAGRAM = 5,
                MISSED_DATAGRAM = 6
               };

    explicit Server(QObject* parent=nullptr);
    virtual ~Server();
    void init(bool udp=true, quint16 port=1234, bool send_heart=false);
    void deinit();

signals:
    void dataReady(const udp_data_t& data);
    void dataReady(const tcp_data_t& data);

private slots:
    void readyReadUdp();
    void handleConnection();

    void route(States state);
    void sendHeartbeat();

private:
    union {
        QUdpSocket* udp;
        QTcpServer* server;
    } m_socket;


    QUdpSocket* m_hearSocket;
    QTimer      m_heartbeat;
    QTimer      m_recTime;
    QHostAddress m_senderHost;
    quint16      m_senderPort;
    bool        m_sendHeart; // inspired by...

    // for the hotswap I may need an auxilary buffer
    // to store the udp packets while closing and creating
    // the new wav files, so nothing will be lost, I`ll do
    // that logic later.

    // may be the client will send me backup packets
    // let`s say 5 so I must store a 5 elements array
    // and to append the  firs occurency I meet, then
    // move to antoher buffer, new concept, unimplemented

};

} // namespace iz

#endif // SERVER_H
