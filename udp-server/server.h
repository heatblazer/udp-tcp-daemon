#ifndef SERVER_H
#define SERVER_H

// qt //
#include <QFile>
#include <QObject>
#include <QTimer>
#include <QTcpSocket>
#include <QUdpSocket>

// custom //
#include "writer.h"
#include "wav-writer.h"

namespace iz {

class Server : public QObject
{
    Q_OBJECT
public:

    explicit Server(QObject* parent=nullptr);
    virtual ~Server();
    void init(bool udp=true, quint16 port=1234);

private slots:
    void readyReadUdp();
    void readyReadTcp();
    void handleConnection();

    void route();
    void sendHeartbeat();
    void writeToChannel(short data[], int len, int chan);

private:
    union {
        QUdpSocket* udp;
        QTcpSocket* tcp;
    } m_socket;
    QUdpSocket* m_hearSocket;
    Writer      m_logger;
    QTimer      m_heartbeat;
    QHostAddress m_senderHost;
    quint16      m_senderPort;
    Wav*   m_wavs[32]; // channels
    static RecConfig* s_conf;

};

} // namespace iz

#endif // SERVER_H
