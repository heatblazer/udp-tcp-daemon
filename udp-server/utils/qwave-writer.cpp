#include "qwave-writer.h"
// unimplemented yet

namespace iz {

bool QWav::isOpened() const
{
    return m_wav.isOpen();
}

void *QWav::read()
{
    return nullptr;
}

void QWav::setupWave(int samples_per_sec, int bits_per_sec, int riff_len,
                     int fmt_len,
                     short audio_fmt,
                     short chann_cnt)
{
    (void) samples_per_sec;
    (void) bits_per_sec;
    (void) riff_len;
    (void) fmt_len;
    (void) audio_fmt;
    (void) chann_cnt;
}

int QWav::write(short data[], int len)
{
    (void) data;
    (void) len;
    return -1;
}

void QWav::close()
{

}

bool QWav::open(const char *perms)
{
    (void) perms;
    return false;
}

QWav::QWav(const QString &fname)
{
    (void) fname;
}

QWav::~QWav()
{

}

} // iz
