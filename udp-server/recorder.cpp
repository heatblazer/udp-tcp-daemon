#include "recorder.h"

// wav library //
#include "utils/wav-writer.h"

namespace iz {

Recorder::Recorder(QObject *parent)
    : QObject(parent)
{

}

// handle with care the opened files
Recorder::~Recorder()
{

}

/// TODO: config file
/// will apply timestapm from the config later
/// \brief Recorder::init
/// \return true by default , false for future if something happens
bool Recorder::init()
{
    bool res = true;
    char buff[16]={0};
    for(int i=0; i < 32; ++i) {
        sprintf(buff, "%d.wav", i+1);
        m_wavs[i] = new Wav(buff);
    }
    // open files when everything is ok and setup
    res &= setupWavFiles();

    for(int i=0; i < 32; ++i) {
        res &= m_wavs[i]->open("wb");
    }

    // remove after tests
    if (res) {
        deinit(); // deleteme
    }
    return res;
}

void Recorder::deinit()
{
    for(int i=0; i < 32; ++i) {
        if (m_wavs[i] != nullptr && m_wavs[i]->isOpened()) {
            m_wavs[i]->close();
            delete m_wavs[i];
            m_wavs[i] = nullptr;
        }
    }
}

/// setup all wav files for writing
/// \brief Recorder::setupWavFiles
/// \return true bu default
bool Recorder::setupWavFiles()
{
    bool res = true;

    int samples_per_sec = 0;
    int bits_per_sec = 0;
    int riff_len = 0;
    int fmt_len = 0;
    short audio_fmt = 0;
    short chann_cnt = 0;

    // careful now: we dont want the parser to fail and to
    // generate bad stuff to our wave header, the
    // parseInt() and parseShort() can return bool for success
    // if we fail it, then we assign our custom default wav header
    // valuse
    PairList& attribs = RecorderConfig::Instance().getTagPairs("Wave");

    for(int i=0; i < attribs.count(); ++i) {
        bool parse_result = false; // careful when converting to int
        MPair<QString, QString> it = attribs.at(i);
        if (it.m_type1 == "samplesPerFrame") {
            samples_per_sec = it[it.m_type1].toInt(&parse_result);
            // failsafe
            if (!parse_result) {
                samples_per_sec = 8000;
            }
        } else if (it.m_type1 == "bitsPerSec") {
            bits_per_sec = it[it.m_type1].toInt(&parse_result);
            if (!parse_result) {
                bits_per_sec = 16;
            }
        } else if (it.m_type1 == "fmtLength") {
            fmt_len = it[it.m_type1].toInt(&parse_result);
            if (!parse_result) {
                fmt_len = 16;
            }
        } else if (it.m_type1 == "audioFormat") {
            audio_fmt = it[it.m_type1].toShort(&parse_result);
            if (!parse_result) {
                audio_fmt = 1;
            }
        } else if (it.m_type1 == "channels") {
            chann_cnt = it[it.m_type1].toShort(&parse_result);
            if (!parse_result) {
                chann_cnt = 1;
            }
        } else if (it.m_type1 == "endiness"){
            // if we require endian swap for the data
        } else {
            // misra else - unused
        }
    }

    // one time setup all waves at a time
    // don`t open them yet... do this later, after server
    // init and binding
    for(int i=0; i < 32; ++i) {
        m_wavs[i]->setupWave(samples_per_sec, bits_per_sec, riff_len,
                             fmt_len, audio_fmt, chann_cnt);
    }

    return res;
}

/// asume the file is opened and setup,
/// now we handle the signals from the server
/// and write the sample data to a specific slot
/// \brief Recorder::record
/// \param data - samples
/// \param slot of the wav file
void Recorder::record(const udp_data_t &data, uint32_t slot)
{
    // more precautions
    if (slot > 31 || m_wavs[slot] == nullptr ||
            !m_wavs[slot]->isOpened()) {
        return; // out of max chan index
    }

    // just calculate the data size based on the known
    // for now udp header, for future we may change the size
    // but if the udp header remains, the above calc will be ok
    uint32_t data_size = (sizeof(data) -
            (sizeof(data.counter) + (sizeof(data.null_bytes)/sizeof(data.null_bytes[0])))
            );

    m_wavs[slot]->write((short*)data.data, data_size);
}

} // iz
