#ifndef WRITER_H
#define WRITER_H

#include <QFile>
#include <QThread>
#include <QMutex>
#include <QQueue>

namespace iz {
class Writer : public QThread
{
    Q_OBJECT
public:
    explicit Writer(const QString& fname, QThread* parent=nullptr);
    virtual ~Writer();
    void write(const QByteArray& data);
    void run() override;

    void startWriter();
    void stopWriter();

private:
    QFile               m_file;
    QMutex              m_mutex;
    QQueue<QByteArray>  m_buffer;
    bool                m_isRunning;

};

} // iz
#endif // WRITER_H
