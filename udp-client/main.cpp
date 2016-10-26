#include <QCoreApplication>
#include "client.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    iz::Client c;
    c.init();
    // transmit 100 udp packets
    for(int i=0; i < 100; ++i) {
        c.transmit();
    }
    return a.exec();
}
