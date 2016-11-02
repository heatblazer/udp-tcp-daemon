#ifndef SAPPLICATION_H
#define SAPPLICATION_H

// parent //
#include <QCoreApplication>

// application stuff //
#include "config.h"
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

    explicit SApplication(int& argc, char** argv);
    virtual ~SApplication();
    int init();

private:
    Server m_server;
    Recorder m_recorder;
    QString m_recConf;
    bool m_setup;
};

} // iz

#endif // SAPPLICATION_H
