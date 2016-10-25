#ifndef WAVWRITER_H
#define WAVWRITER_H

#include <stdio.h>
#include <stdint.h>

namespace iz {

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


class Wav
{

public:
    Wav(const char* fname);
    ~Wav();
    virtual bool open(const char* perms);
    virtual void close();
    virtual int write(short int data[], int len);
private:
    void write_hdr();
private:
    FILE*   m_file;
    char m_filename[64];
};

} // iz


#endif // WAVWRITER_H
