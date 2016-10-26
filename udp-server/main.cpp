#include "sapplication.h"
#include "daemon.h"

int main(int argc, char *argv[])
{
    iz::daemonize();
    iz::SApplication app(argc, argv);
    app.init();
    return app.exec();
}
