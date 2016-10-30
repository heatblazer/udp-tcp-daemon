#ifndef CLIENT_H
#define CLIENT_H

// qt //
#include <QTimer>
#include <QObject>
#include <QUdpSocket>

//defs //
#include "defs.h"

namespace iz {

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject* parent=nullptr);
    ~Client();
    void init();

public slots:

    void transmit();
private:
    QTimer m_timer;
    QUdpSocket* p_socket;
    char    m_data[196];
};

}


#endif // CLIENT_H
