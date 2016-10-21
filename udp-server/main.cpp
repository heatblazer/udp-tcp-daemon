#include <QCoreApplication>
#include "server.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    iz::Server s;
    s.init(true);

    return a.exec();
}
