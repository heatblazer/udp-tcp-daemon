#include "wav-writer.h"

#include <iostream>

namespace iz {

Wav::Wav()
{
    m_data.reserve(sizeof(struct WavHdr));
    std::cout << "WavHdr: " << sizeof(struct WavHdr) << std::endl;
}

Wav::~Wav()
{
}

void Wav::writeHeader(const WavHdr* hdr)
{
    char wh[44] = {0};
    memcpy(wh, hdr, sizeof(WavHdr));
    m_data.append(wh);
}

void Wav::appendData(const QByteArray &data)
{
    m_data.append(data);
}

QByteArray Wav::getWavData()
{
    return m_data;
}

}  // iz
