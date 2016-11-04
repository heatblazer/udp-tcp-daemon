#ifndef WAVWRITER_H
#define WAVWRITER_H

// ansi c //
#include <stdio.h>
#include <stdint.h>


/// the default wav header
/// \brief The wav_hdr_t struct

struct wav_hdr_t
{
    char riff_tag[4];
    int  riff_len;
    char wav_tag[4];
    char fmt_tag[4];
    int fmt_len;
    short audio_format;
    short num_channels;
    int sample_rate;
    int byte_rate;
    short block_align;
    short bits_per_sample;
    char data_tag[4];
    int data_len;
};

namespace iz {

/// a minimal and portable
/// wav file writer C library
/// \brief The Wav class
class Wav
{
public:
    enum Params {
        SAMPLES_PER_FRAME,
        BITS_PER_SEC,
        RIFF_LEN,
        AUD_FORMAT,
        CHANNELS,
        SIZE
    };

public:
    Wav(const char* fname);
    virtual ~Wav();
    // in case we migrate to other file api we made these virtual
    // but provide some defailt implementation
    virtual bool open(const char* perms);
    virtual void close();
    virtual bool isOpened() const ;
    virtual int write(short int data[], int len);
    virtual void setupWave(int samples_per_sec=8000, int bits_per_sec=16, int riff_len=0,
                            int fmt_len=16, short audio_fmt=1,  short chann_cnt=1);

public:
    /// using Daemon fwrite
    /// \brief write_hdr
    /// writes a proper wav header, has
    /// set default parameters
    /// it`s virtual because you may
    /// want to override the write function
    /// \param spf - samples per frm
    /// \param bps - bits per second
    /// \param rifflen - riff length
    /// \param fmtlen - format length
    /// \param audfmt - audo format
    /// \param chans - channels count
    virtual void write_hdr(int spf=44100, int bps=16, int rifflen=0, int fmtlen=16, short audfmt=1, short chans=1);
    const char* getFileName();

protected:
    FILE*   m_file;
    wav_hdr_t m_header;
    char m_filename[64];
    // to be deprecated
    int m_attribs[Params::SIZE];
    bool m_isSetup;
    bool m_isOpened;
};


} // iz


#endif // WAVWRITER_H
