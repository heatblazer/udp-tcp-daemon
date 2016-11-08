#ifndef QWAVEWRITER_H
#define QWAVEWRITER_H

// qt stuff //
#include <QFile>

#include "types.h"
#include "wav-writer-iface.h"

namespace iz {
/// newer concept for wave writer using QT
/// \brief The QWave class
///
class QWav : public WavIface
{
public:
    explicit QWav(const QString& fname);
    virtual ~QWav();
    virtual bool open(const char* perms) = 0;
    virtual void close() = 0;
    virtual int write(short int data[], int len) = 0;
    virtual void setupWave(int samples_per_sec, int bits_per_sec, int riff_len,
                            int fmt_len, short audio_fmt,  short chann_cnt) = 0;
    virtual void* read() = 0;
    virtual bool isOpened() const = 0;
private:
    QString m_name;
    QFile m_wav;
    int   m_slot;
    bool  m_setup;
    wav_hdr_t m_header;

};


} // iz


#endif // QWAVEWRITER_H
