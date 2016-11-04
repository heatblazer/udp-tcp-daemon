#ifndef SAPPLICATION_H
#define SAPPLICATION_H

// parent //
#include <QCoreApplication>

// application stuff //
#include "server.h"
#include "utils/wav-writer.h"
#include "utils/recorder-config.h"
#include "recorder.h"

namespace iz {

// application will connect
// server and recorder via
// signals and slots
class SApplication : public QCoreApplication
{
public:

    static void writeToSocket(const char* data);

    explicit SApplication(int& argc, char** argv);
    virtual ~SApplication();
    int init();
    void deinit();

public:
    // the socket concpet for the signal handlers
    static int m_fdHUP;
    static int m_fdTERM;

private:
    Server m_server;
    Recorder m_recorder;
    QString m_recConf;
    bool m_setup;
};

} // iz

#endif // SAPPLICATION_H
