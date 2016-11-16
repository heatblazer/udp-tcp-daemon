#ifndef LOGGER_H
#define LOGGER_H

// parent //
#include <QObject>

// component //
#include "writer.h"

namespace iz {

class Logger : public QObject
{
    Q_OBJECT
public:
    static Logger& Instance();
    bool init();
    void deinit();
    void logMessage(const char* msg);

private:
    explicit Logger(QObject* parent=nullptr);
    virtual ~Logger();
    Writer m_writer;

    static Logger* s_inst;
};


} // iz


#endif // LOGGER_H
