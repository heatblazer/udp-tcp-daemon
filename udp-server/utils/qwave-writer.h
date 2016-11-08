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
    virtual bool open(const char* perms);
    virtual void close();
    virtual int write(short int data[], int len);
    virtual void setupWave(int samples_per_sec=8000, int bits_per_sec=16, int riff_len=0,
                           int fmt_len=16, short audio_fmt=1,  short chann_cnt=1);
    virtual void* read();
    virtual bool isOpened() const;

    const QString&  getFileName();
    size_t  getFileSize() const;
    bool open(const char* perms, int slot);
    int getSlot() const;

private:
    QString m_name;
    QFile m_wav;
    int   m_slot;
    bool  m_setup;
    size_t m_size;
    wav_hdr_t m_header;

};


} // iz


#endif // QWAVEWRITER_H
