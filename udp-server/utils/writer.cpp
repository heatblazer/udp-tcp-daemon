#include "writer.h"

#include <QDir>
#include <QTextStream>

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

Writer::Writer(QThread *parent)
    : QThread(parent),
      m_isRunning(false),
      m_speed(1000)
{

}

Writer::~Writer()
{
}

bool Writer::setup()
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
    if (speed.m_type1 != "") {
        bool res = false;
        m_speed = speed.m_type2.toULong(&res);
        if (!res) {
            m_speed = 1000;
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

    m_buffer.reserve(100);

    if (!m_file.isOpen()) {
        m_file.setFileName(fname);
        m_file.open(QIODevice::Append|QIODevice::ReadWrite);
    }
    return res;
}

void Writer::write(const QByteArray &data)
{
    m_mutex.lock();
    m_buffer.append(data);
    m_mutex.unlock();
}

void Writer::run()
{
    QQueue<QByteArray> dblBuff;
    dblBuff.reserve(100);
    do {
        usleep(m_speed);

        m_mutex.lock();
        while (!m_buffer.isEmpty()) {
            dblBuff.enqueue(m_buffer.dequeue());
        }
        m_mutex.unlock();

        while (!dblBuff.isEmpty()) {
            QByteArray d = dblBuff.dequeue();
            m_file.write(d, d.size());
            m_file.flush();
        }

    } while (m_isRunning);
}

void Writer::startWriter()
{
    m_isRunning = true;
    start();
}

void Writer::stopWriter()
{
    m_isRunning = false;
    wait(1000); // qt`s variant for joining
}

} // iz
