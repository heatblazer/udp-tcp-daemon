#include "sapplication.h"

int main(int argc, char *argv[])
{
    iz::SApplication app(argc, argv);
    app.init();
    return app.exec();
}
