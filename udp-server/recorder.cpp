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
bool Recorder::init(int hot_swap)
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
        res &= m_wavs[i]->open("wb");
        if (res) {
            m_filewatcher.addPath(m_wavs[i]->getFileName());
        }
    }
    connect(&m_filewatcher, SIGNAL(fileChanged(QString)),
            this, SLOT(testFileWatcher(QString)));

    m_hotswap.setInterval(hot_swap);
    connect(&m_hotswap, SIGNAL(timeout()),
            this, SLOT(hotSwapFiles()));
    if (res) {
        m_hotswap.start();
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
    m_hotswap.stop();
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
    for(int i=0; i < 32; ++i) {
        if (m_wavs[i] != nullptr && m_wavs[i]->isOpened()) {
            m_wavs[i]->write((short*) data.data[i], 16);
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

// TODO: implement the swap logic
void Recorder::testFileWatcher(const QString &file)
{
    (void) file;
    for(int i=0; i < 32; ++i) {
        if (m_wavs[i] != nullptr && m_wavs[i]->isOpened()) {
            if (m_wavs[i]->getFileSize() > m_maxFileSize) {
                char buff[64]={0};
                sprintf(buff, "%d-%s.wav",
                        i, getTimeString());
                m_wavs[i]->close();
                delete m_wavs[i];
                m_wavs[i] = nullptr;
                m_wavs[i] = new Wav(buff);
            }
        }
    }
}

} // iz
