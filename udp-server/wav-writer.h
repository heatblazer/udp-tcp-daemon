#ifndef WAVWRITER_H
#define WAVWRITER_H

// ansi c //
#include <stdio.h>
#include <stdint.h>

#include "config.h"

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
    Wav(const char *fname, WavConfig* cfg);
    virtual ~Wav();
    // in case we migrate to other file api we made these virtual
    // but provide some defailt implementation
    virtual bool open(const char* perms);
    virtual void close();
    virtual int write(short int data[], int len);

protected:
    /// using UNIX fwrite
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

private:
    FILE*   m_file;
    WavConfig* m_conf;
    char m_filename[64];
    int m_attribs[Params::SIZE];
};


} // iz


#endif // WAVWRITER_H
