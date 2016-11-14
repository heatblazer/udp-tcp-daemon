#include "writer.h"

#include <QTextStream>

#include "recorder-config.h"

namespace iz {

Writer::Writer(const QString &fname, QThread *parent)
    : QThread(parent),
      m_isRunning(false),
      m_speed(1000)
{
    m_buffer.reserve(100);
    if (!m_file.isOpen()) {
        m_file.setFileName(fname);
        m_file.open(QIODevice::Append|QIODevice::ReadWrite);
    }
}

Writer::~Writer()
{
}

bool Writer::setup()
{
    bool res = true;
    // TODO: fix later
#if 0
    const MPair<QString, QString> name = RecorderConfig::Instance()
            .getAttribPairFromTag("Log", "name");

    const MPair<QString, QString> path = RecorderConfig::Instance()
            .getAttribPairFromTag("Log", "path");

    const MPair<QString, QString> time = RecorderConfig::Instance()
            .getAttribPairFromTag("Log", "time");

    const MPair<QString, QString> speed = RecorderConfig::Instance()
            .getAttribPairFromTag("Log", "speed");
#endif
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
