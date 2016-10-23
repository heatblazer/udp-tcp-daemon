#ifndef SERVER_H
#define SERVER_H

// qt //
#include <QFile>
#include <QObject>
#include <QTcpSocket>
#include <QUdpSocket>

// unix //
#include <signal.h>


// custom //
#include "writer.h"
#include "wav-writer.h"

namespace iz {

typedef void (*sigHndl)(int, siginfo_t *,void*);

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
    void init(bool is_daemon=false, bool udp=true, quint16 port=7755);

private slots:
    void readyReadUdp();
    void readyReadTcp();

private:
    static void daemonize();
    static void attachSignalHandler(sigHndl hnd, int slot);

private:
    union {
        QUdpSocket* udp;
        QTcpSocket* tcp;
    } m_socket;

    Writer      m_wavWriter;
    Writer      m_logger;
    // remove it after tests //
    Wav         m_test;
    static struct sigaction m_signals[32];

    static Server* s_instance; // set after init
};

} // namespace iz

#endif // SERVER_H
