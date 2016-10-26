#ifndef SERVER_H
#define SERVER_H

// qt //
#include <QFile>
#include <QObject>
#include <QTimer>
#include <QUdpSocket>

// custom //
#include "writer.h"
#include "wav-writer.h"

namespace iz {


struct UdpHdr
{
    union {
        qint16 i;
        char c[sizeof(qint16)];
    } port;

    union {
        qint16 i;
        char c[sizeof(qint16)];
    } info;
};

class Server : public QObject
{
    Q_OBJECT
public:

    explicit Server(QObject* parent=nullptr);
    virtual ~Server();
    void init(bool udp=true, quint16 port=12345);

private slots:
    void readyReadUdp();
    void readyReadTcp();
    void handleConnection();

private slots:
    void route();
    void sendHeartbeat();

private:
    QUdpSocket* m_socket;
    Writer      m_logger;
    QTimer      m_hearbeat;

};

} // namespace iz

#endif // SERVER_H
