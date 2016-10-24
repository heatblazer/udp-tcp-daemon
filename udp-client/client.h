#ifndef CLIENT_H
#define CLIENT_H
// qt //
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

    QUdpSocket* p_socket;
};

}


#endif // CLIENT_H
