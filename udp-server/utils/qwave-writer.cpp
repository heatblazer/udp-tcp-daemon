#include "qwave-writer.h"
// unimplemented yet

namespace iz {

bool QWav::isOpened() const
{
    return m_wav.isOpen();
}

/// unimplemented
/// \brief QWav::read
/// \return
///
void *QWav::read()
{
    return nullptr;
}

void QWav::setupWave(int samples_per_sec,
                     int bits_per_sec,
                     int riff_len,
                     int fmt_len,
                     short audio_fmt,
                     short chann_cnt)
{
    m_header.riff_tag[0] = 'R';
    m_header.riff_tag[1] = 'I';
    m_header.riff_tag[2] = 'F';
    m_header.riff_tag[3] = 'F';

    m_header.wav_tag[0] = 'W';
    m_header.wav_tag[1] = 'A';
    m_header.wav_tag[2] = 'V';
    m_header.wav_tag[3] = 'E';

    m_header.fmt_tag[0] = 'f';
    m_header.fmt_tag[1] = 'm';
    m_header.fmt_tag[2] = 't';
    m_header.fmt_tag[3] = ' ';

    m_header.data_tag[0] = 'd';
    m_header.data_tag[1] = 'a';
    m_header.data_tag[2] = 't';
    m_header.data_tag[3] = 'a';

    m_header.riff_len = riff_len;
    m_header.fmt_len = fmt_len;
    m_header.audio_format = audio_fmt;
    m_header.num_channels = chann_cnt;
    m_header.sample_rate = samples_per_sec;
    m_header.byte_rate = samples_per_sec * (bits_per_sec/8);
    m_header.block_align = bits_per_sec / 8;
    m_header.bits_per_sample = bits_per_sec;
    m_header.data_len = 0;

    m_setup = true;

}

int QWav::write(short data[], int len)
{
    m_size += m_wav.write((char*) data, len);
    return m_size;
}

void QWav::close()
{

}

bool QWav::open(const char *perms)
{
    (void) perms;

    if (m_wav.exists()) {
        return false;
    }
    m_wav.setFileName(m_name);
    if (!m_wav.isOpen()) {
        m_wav.open(QIODevice::WriteOnly);
    }
    // we have not called setup! Load some defaults !
    if (!m_setup) {
        // write default header
        setupWave();
    }
    m_wav.write((char*)&m_header, sizeof(m_header));
    m_wav.flush();
    m_size += 44;
    return m_wav.isOpen();
}

QWav::QWav(const QString &fname)
    : m_name(fname),
      m_slot(-1),
      m_setup(false),
      m_size(0)
{

}

QWav::~QWav()
{

}

const QString &QWav::getFileName()
{
    return m_name;
}

size_t QWav::getFileSize() const
{
    return m_size;
}

int QWav::getSlot() const
{
    return m_slot;
}

} // iz
