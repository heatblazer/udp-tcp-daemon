#ifndef SAPPLICATION_H
#define SAPPLICATION_H
// parent //
#include <QCoreApplication>


// server //
#include "server.h"

// wav writer //
#include "wav-writer.h"

namespace iz {

class SApplication : public QCoreApplication
{
public:
    explicit SApplication(int& argc, char** argv);
    virtual ~SApplication();

    int init();

private:
    Server m_server;
    Wav*   m_wavs[32];
    QWav*  test_wav;

};

} // iz

#endif // SAPPLICATION_H
