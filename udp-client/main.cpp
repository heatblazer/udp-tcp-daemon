#include <QCoreApplication>
#include "client.h"
#include "wav-writer.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    iz::Client c;
    c.init();
    iz::WavWriter::load("assets/1.wav", "rb");
    (void)iz::WavWriter::readRawData();
    iz::WavWriter::unload();
    // transmit 100 udp packets
    for(int i=0; i < 100; ++i) {
        c.transmit();
    }
    return a.exec();
}
