#ifndef WAVWRITER_H
#define WAVWRITER_H

#include <QByteArray>

namespace iz {
struct WavHdr
{
    char riff[4];
    qint32 file_size;
    char wave[4];
    char fmt[4];
    char len[3];
    qint16 type;
    quint16 format;
    qint32 sample_rate;
    qint32 sr_bs_channs;
    quint8 bits_per_sample;
    char data[4];
    qint32 fsize;
};

class Wav
{

public:
    Wav();
    ~Wav();
    void writeHeader(const WavHdr *hdr);
    void appendData(const QByteArray& data);
    QByteArray getWavData();

private:
    QByteArray m_data;

};

} // iz


#endif // WAVWRITER_H
