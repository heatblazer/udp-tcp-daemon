#include "recorder.h"

// remove later
#include <iostream>

// wav library //
#include "utils/wav-writer.h"


/// timestring
/// \brief getTimeString
/// \return
///
static inline char* getTimeString()
{
    time_t current_time;
    struct tm * time_info;
    static char timeString[9];  // space for "HH:MM:SS\0"

    time(&current_time);
    time_info = localtime(&current_time);
    strftime(timeString, sizeof(timeString), "%H:%M:%S", time_info);
    return timeString;
}


namespace iz {

struct udp_data_t
{
    uint32_t    counter;
    uint8_t     null_bytes[32];
    int16_t    data[32][16];
    //int16_t    data[16][32];
};

struct tcp_data_t
{
    int16_t data[128];
};


Recorder::Recorder(QObject *parent)
    : QObject(parent),
      m_maxFileSize(0)
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

    char buff[64]={0};
    for(int i=0; i < 32; ++i) {
        sprintf(buff, "%d-%s.wav", i,
                getTimeString());
        m_wavs[i] = new Wav(buff);
    }
    // open files when everything is ok and setup
    res &= setupWavFiles();

    for(int i=0; i < 32; ++i) {
        res &= m_wavs[i]->open("wb", i);
        if (res) {
            m_filewatcher.addPath(m_wavs[i]->getFileName());
        }
    }
#if 0
    connect(&m_filewatcher, SIGNAL(fileChanged(QString)),
            this, SLOT(testFileWatcher(QString)));
#endif
    // test the new concept for hotswapping !
    connect(&m_filewatcher, SIGNAL(fileChanged(QString)),
            this, SLOT(performHotSwap(QString)));

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

Wav* Recorder::getWavByName(const QString &fname)
{
    for(int i=0; i < 32; ++i) {
        if (fname.compare(QString(m_wavs[i]->getFileName()))) {
            return m_wavs[i];
        }
    }
    return nullptr;
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

    PairList& max_fsize_attr = RecorderConfig::Instance().getTagPairs("HotSwap");
    for(int i=0; i < max_fsize_attr.count(); ++i) {
        bool parse_result = false; // careful when converting to int
        MPair<QString, QString> it = max_fsize_attr.at(i);
        if (it.m_type1 == "maxSize") {
            m_maxFileSize = it[it.m_type1].toInt(&parse_result);
            if (!parse_result) {
                m_maxFileSize = 250000000;
            }
        } else if (it.m_type1 == "interval") {
            if (!parse_result) {
                // omit for now
            }
        } else {
            // misra else
        }
    }

    // one time setup all waves at a time
    // don`t open them yet... do this later, after server
    // init and binding
    for(int i=0; i < 32; ++i) {
        m_wavs[i]->setupWave(samples_per_sec, bits_per_sec, riff_len,
                             fmt_len, audio_fmt, chann_cnt);
    }
    m_wavParams.samples_per_sec = samples_per_sec;
    m_wavParams.bits_per_sec = bits_per_sec;
    m_wavParams.riff_len = riff_len;
    m_wavParams.fmt_len = fmt_len;
    m_wavParams.audio_fmt = audio_fmt;
    m_wavParams.chann_cnt = chann_cnt;

    return res;
}

/// asume the file is opened and setup,
/// now we handle the signals from the server
/// and write the sample data to a specific slot
/// \brief Recorder::record
/// \param data - samples
/// \param slot of the wav file
void Recorder::record(const udp_data_t &data)
{
    // flip the data
#if OK_PASSED_TEST
    int16_t flip_data[32][16] = {0};

    for(int i=0; i < 16; ++i) {
        for(int j=0; j < 32; ++j) {
            flip_data[j][i] = data.data[i][j];
        }
    }
#endif
    for(int i=0; i < 32; ++i) {
        if (m_wavs[i] != nullptr && m_wavs[i]->isOpened()) {
            // TODO: test!
#ifdef OK_PASSED_TEST
            m_wavs[i]->write((short*) flip_data[i], 16);
#else
            m_wavs[i]->write((short*) data.data[i], 16);
#endif
        }
    }
}

// unused for now
void Recorder::record(const tcp_data_t &data)
{
    (void) data;
    std::cout << "Stub: Record TCP!" << std::endl;
}

/// stub
/// \brief Recorder::hotSwapFiles
/// hotswaps wave files with new ones when
/// a given recorder time elapses
void Recorder::hotSwapFiles()
{
    std::cout << "hotSwapFiles: stub!" << std::endl;
}

// this is a test but working for now
// to be deprecated in the future
void Recorder::testFileWatcher(const QString &file)
{
    (void) file;
    for(int i=0; i < 32; ++i) {
        if (m_wavs[i] != nullptr && m_wavs[i]->isOpened()) {
            if (m_wavs[i]->getFileSize() > m_maxFileSize) {
                m_filewatcher.removePath(m_wavs[i]->getFileName());
                char buff[64] = {0};
                sprintf(buff, "%d-%s.wav",
                        i, getTimeString());
                m_wavs[i]->close();
                delete m_wavs[i];
                m_wavs[i] = nullptr;
                m_wavs[i] = new Wav(buff);
                m_wavs[i]->setupWave(m_wavParams.samples_per_sec,
                                     m_wavParams.bits_per_sec,
                                     m_wavParams.riff_len,
                                     m_wavParams.fmt_len,
                                     m_wavParams.audio_fmt,
                                     m_wavParams.chann_cnt);
                m_wavs[i]->open("wb", i);
                m_filewatcher.addPath(m_wavs[i]->getFileName());
            }
        }
    }
}

void Recorder::performHotSwap(const QString &file)
{
    Wav* w = getWavByName(file);
    if (w != nullptr) {
        if (w->getFileSize() > m_maxFileSize) {
            const char* name = w->getFileName();
            m_filewatcher.removePath(name);
            int slot = w->getSlot();
            m_wavs[slot]->close();
            delete m_wavs[slot];
            m_wavs[slot] = nullptr;
            char buff[64] = {0};
            sprintf(buff, "%d-%s.wav",
                    slot, getTimeString());

            m_wavs[slot] = new Wav(buff);
            m_wavs[slot]->setupWave(m_wavParams.samples_per_sec,
                                    m_wavParams.bits_per_sec,
                                    m_wavParams.riff_len,
                                    m_wavParams.fmt_len,
                                    m_wavParams.audio_fmt,
                                    m_wavParams.chann_cnt);
            m_wavs[slot]->open("wb", slot);
            m_filewatcher.addPath(buff);
        }
    }
}

} // iz
