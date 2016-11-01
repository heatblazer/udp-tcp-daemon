#include "recorder.h"

// wav library //
#include "wav-writer.h"

namespace iz {

Recorder::Recorder(QObject *parent)
    : QObject(parent)
{

}

Recorder::~Recorder()
{
    // cleanup()
}

/// TODO: config file
/// will apply timestapm from the config later
/// \brief Recorder::init
/// \return
///
bool Recorder::init()
{
    char buff[16]={0};
    for(int i=0; i < 32; ++i) {
        sprintf(buff, "%d.wav", i+1);
        m_wavs[i] = new Wav(buff);
    }

    return true;
}

bool Recorder::setupWavFiles()
{
    int samples_per_sec = 0;
    int bits_per_sec = 0;
    int riff_len = 0;
    int fmt_len = 0;
    short audio_fmt = 0;
    short chann_cnt = 0;

    PairList& attribs = RecorderConfig::Instance().getTagPairs("Wave");
    for(int i=0; i < attribs.count(); ++i) {
        bool parse_result = false; // careful when converting to int
        MPair<QString, QString> it = attribs.at(i);
        if (it.m_type1 == "samplesPerFrame") {
            samples_per_sec = it[it.m_type1]->toInt(&parse_result);
            // failsafe
            if (!parse_result) {
                samples_per_sec = 8000;
            }
        } else if (it.m_type1 == "bitsPerSec") {
            bits_per_sec = it[it.m_type1]->toInt(&parse_result);
            if (!parse_result) {
                bits_per_sec = 16;
            }
        } else if (it.m_type1 == "fmtLength") {
            fmt_len = it[it.m_type1]->toInt(&parse_result);
            if (!parse_result) {
                fmt_len = 16;
            }
        } else if (it.m_type1 == "audioFormat") {
            audio_fmt = it[it.m_type1]->toShort(&parse_result);
            if (!parse_result) {
                audio_fmt = 1;
            }
        } else if (it.m_type1 == "channels") {
            chann_cnt = it[it.m_type1]->toShort(&parse_result);
            if (!parse_result) {
                chann_cnt = 1;
            }
        } else {
            // nothing
        }
    }

    for(int i=0; i < 32; ++i) {
        m_wavs[i]->setupWave(samples_per_sec, bits_per_sec, riff_len,
                             fmt_len, audio_fmt, chann_cnt);
    }

    return true;
}

void Recorder::record(const udp_data_t &data, uint32_t slot)
{
    if (slot > 31) {
        return; // out of max chan index
    }
    uint32_t data_size = (sizeof(data) -
            (sizeof(data.counter) + (sizeof(data.null_bytes)/sizeof(data.null_bytes[0])))
            );

    m_wavs[slot]->write((short*)data.data, data_size);
}

} // iz
