#ifndef SAPPLICATION_H
#define SAPPLICATION_H

// parent //
#include <QCoreApplication>

// application stuff //
#include "config.h"
#include "server.h"
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
    RecConfig m_config;
};

} // iz

#endif // SAPPLICATION_H
