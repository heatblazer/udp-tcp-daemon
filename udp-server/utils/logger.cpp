#include "logger.h"

// qt //
#include <QDir>

// local stuff //
#include "recorder-config.h"


/// timestring
/// \brief getTimeString
/// \return
///
static inline char* getTimeString()
{
    time_t current_time;
    struct tm * time_info;
    static char timeString[9];  // space for "HH:MM:SS\0"

    time(&current_time);
    time_info = localtime(&current_time);
    strftime(timeString, sizeof(timeString), "%H:%M:%S", time_info);
    return timeString;
}

namespace iz {

Logger* Logger::s_inst = nullptr;

Logger::Logger(QObject *parent)
    : QObject(parent)
{

}

Logger::~Logger()
{

}

Logger &Logger::Instance()
{
    if (s_inst == nullptr) {
        s_inst = new Logger;
    }
    return *s_inst;
}

bool Logger::init()
{
    bool res = true;

    const MPair<QString, QString> log_dir = RecorderConfig::Instance()
            .getAttribPairFromTag("Paths", "logs");

    const MPair<QString, QString> name = RecorderConfig::Instance()
            .getAttribPairFromTag("Log", "name");

    const MPair<QString, QString> time = RecorderConfig::Instance()
            .getAttribPairFromTag("Log", "timestamp");

    const MPair<QString, QString> speed = RecorderConfig::Instance()
            .getAttribPairFromTag("Log", "speed");

    char fname[256] = {0};

    if (log_dir.m_type1 != "") {
        if (!QDir(log_dir.m_type2).exists()) {
            QDir().mkdir(log_dir.m_type2);
            sprintf(fname,"%s/", log_dir.m_type2.toStdString().data());
        } else {
            sprintf(fname,"%s/", log_dir.m_type2.toStdString().data());
        }
    }

    // setup logger logging speed
    ulong log_speed = 0;
    if (speed.m_type1 != "") {
        bool res = false;
        log_speed = speed.m_type2.toULong(&res);
        if (!res) {
            log_speed = 1000;
        }
    }

    // setup timestamp
    if (time.m_type1 != "") {
        if (time.m_type2 == "enabled" || time.m_type2 == "true") {
            strcat(fname, getTimeString());
            strcat(fname, "-");
        }
    }

    // setup filename
    if (name.m_type1 != "") {
        if (name.m_type2 != "") {
            strcat(fname, name.m_type2.toStdString().data());
        }
    }

    if (m_writer.setup(QString(fname), 100, log_speed)) {
        m_writer.setObjectName("logger thread");
        m_writer.startWriter();
    }

    return res;
}

void Logger::deinit()
{
    m_writer.stopWriter();
}

void Logger::logMessage(const char *msg)
{
    m_writer.write(QByteArray(msg));
}

} // iz
