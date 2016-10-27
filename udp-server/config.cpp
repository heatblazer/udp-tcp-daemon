#include "config.h"

// text strm for first appending //
#include <QTextStream>


namespace iz {
RecConfig::RecConfig(const QString &fname)
    : m_file{(fname)}
{
    m_file.file.setFileName(m_file.name);

    if(!m_file.file.exists()) {
        m_file.file.open(QIODevice::ReadWrite);
        QTextStream strm(&m_file.file);
        strm << "SAMPLES_PER_FRAME : 44100" << endl;
        strm << "BITS_PER_SEC : 16" << endl;
        strm << "AUD_FORMAT : 1" << endl;
        strm << "CHANNELS : 1" << endl;
        m_file.file.flush();
        m_file.file.close();
    } else {
        m_file.file.open(QIODevice::ReadOnly);
        // read the fields
        QByteArray line;
        do {
            line = m_file.file.readLine().trimmed();
            if (!line.contains(":")) {
                continue;
            }
            if (line.contains("SAMPLES_PER_FRAME")) {
                m_attribs[SAMPLES_PER_FRAME] = line.split(':').at(1).toInt();
            } else if (line.contains("BITS_PER_SEC")) {
                m_attribs[BITS_PER_SEC] = line.split(':').at(1).toInt();
            } else if (line.contains("AUD_FORMAT")) {
                m_attribs[AUD_FORMAT] = line.split(':').at(1).toInt();
            } else if (line.contains("CHANNELS")) {
                m_attribs[CHANNELS] = line.split(':').at(1).toInt();
            }
        } while (!line.isEmpty());

    }
}

RecConfig::~RecConfig()
{

}

int RecConfig::getAttribute(RecConfig::Params atrib)
{
    if (atrib < 0 || atrib > Params::SIZE) {
        return -1;
    }
    return m_attribs[atrib];
}

} // iz
