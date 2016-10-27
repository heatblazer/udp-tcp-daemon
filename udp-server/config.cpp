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
            // skip inproper lines
            if (!line.contains(":")) {
                continue;
            }
            // this is safety check because toInt
            // may fail if the element is not parsable
            int param ;
            bool res ;
            if (line.contains("SAMPLES_PER_FRAME")) {
                param = line.split(':').at(1).toInt(&res);
                if (res) {
                    m_attribs[SAMPLES_PER_FRAME] = param;
                } else {
                    m_attribs[SAMPLES_PER_FRAME] = -1;
                }
            } else if (line.contains("BITS_PER_SEC")) {
                param = line.split(':').at(1).toInt(&res);
                if (res) {
                    m_attribs[BITS_PER_SEC] = param;
                } else {
                    m_attribs[BITS_PER_SEC] = -1;
                }
            } else if (line.contains("AUD_FORMAT")) {
                param = line.split(':').at(1).toInt(&res);
                if (res) {
                    m_attribs[AUD_FORMAT] = param;
                } else {
                    m_attribs[AUD_FORMAT] = -1;
                }
            } else if (line.contains("CHANNELS")) {
                param = line.split(':').at(1).toInt(&res);
                if (res) {
                    m_attribs[CHANNELS] = param;
                } else {
                    m_attribs[CHANNELS] = -1;
                }
            } else {
                // nothing
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
