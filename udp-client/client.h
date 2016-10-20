#ifndef CLIENT_H
#define CLIENT_H
#include <QObject>
#include <QUdpSocket>

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
