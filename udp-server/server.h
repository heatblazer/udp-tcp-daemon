#ifndef SERVER_H
#define SERVER_H
// qt //
#include <QObject>
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
    void init(bool udp=true, quint16 port=7755);

private slots:
    void readyReadUdp();
    void readyReadTcp();

private:
    union {
        QUdpSocket* udp;
        QTcpSocket* tcp;
    } m_socket;

    Writer      m_wavWriter;
    Writer      m_logger;
    Wav         m_test;
};

} // namespace iz

#endif // SERVER_H
