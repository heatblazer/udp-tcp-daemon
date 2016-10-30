#include "defs.h"
#include "wav-writer.h"

#include <string.h> // strcpy
#include <fcntl.h>
#include <unistd.h>


namespace iz {

/// flip endiness 32 bit
/// on big endian machines
/// \brief flip32
/// \param input
/// \return
///
static int32_t flip32(int input)
{
    (void)flip32;
    int32_t output=0;
    uint8_t* s = reinterpret_cast<uint8_t*>(&input);
    uint8_t* d = reinterpret_cast<uint8_t*>(&output);
    *d++ = *(s+3);
    *d++ = *(s+2);
    *d++ = *(s+1);
    *d++ = *(s);
    return output;
}

/// flip endiness 16 bit
/// \brief flip16
/// \param input
/// \return
///
static int16_t flip16(int16_t input)
{
    (void)flip16;
    int16_t output = 0;
    uint8_t* s = reinterpret_cast<uint8_t*>(&input);
    uint8_t* d = reinterpret_cast<uint8_t*>(&output);
    *d++ = *(s+1);
    *d++ = *(s);
    return output;
}

// remove later, these are used since my open, close and
// write, shadows the IDE to match the one in unsitd and
// fcntl.h
#ifndef UNUSED
#else
static int open_file(const char *__file, int __oflag)
{
    return open(__file, __oflag);
}

static int write_file(int fd, void* data, int count)
{
    return write(fd, data, count);
}

static void close_file(int fd)
{
    close(fd);
}
#endif

Wav::Wav(const char *fname)
    : m_file(NULL),
      m_conf(NULL)
{
    strcpy(m_filename, fname);
}

Wav::Wav(const char *fname, RecConfig *cfg)
    : m_file(NULL),
      m_conf(cfg)
{
    strcpy(m_filename, fname);
}

Wav::~Wav()
{
}

bool Wav::open(const char *perms)
{
    m_file = fopen(m_filename, perms);
    if (m_file == NULL) {
        return false;
    }


    if (m_conf == NULL) {
        write_hdr();
    } else {
        write_hdr(m_conf->getAttribute(RecConfig::SAMPLES_PER_FRAME),
                  m_conf->getAttribute(RecConfig::BITS_PER_SEC),
                  0,
                  m_conf->getAttribute(RecConfig::RIFF_LEN),
                  m_conf->getAttribute(RecConfig::AUD_FORMAT),
                  m_conf->getAttribute(RecConfig::CHANNELS));
    }
    return true;
}

void Wav::close()
{
    int file_len = ftell(m_file);

    // get the len from the offset
    int data_len = file_len - sizeof(struct wav_hdr_t);

    // move the fp to that position of the data len
    fseek(m_file, sizeof(struct wav_hdr_t) - sizeof(int), SEEK_SET);
    fwrite(&data_len, sizeof(data_len), 1, m_file);
    // this writes riff len to the position in
    // the header
    int riff_len = file_len - 8;
    fseek(m_file, 4, SEEK_SET);
    fwrite(&riff_len, sizeof(riff_len), 1, m_file);
    fclose(m_file);

}

int Wav::write(short data[], int len)
{
    size_t written = fwrite(data, sizeof(short), len, m_file);
    return (int)written;
}

void Wav::write_hdr(int spf, int bps, int rifflen, int fmtlen, short audfmt, short chans)
{
    struct wav_hdr_t hdr;
    hdr.riff_tag[0] = 'R';
    hdr.riff_tag[1] = 'I';
    hdr.riff_tag[2] = 'F';
    hdr.riff_tag[3] = 'F';

    hdr.wav_tag[0] = 'W';
    hdr.wav_tag[1] = 'A';
    hdr.wav_tag[2] = 'V';
    hdr.wav_tag[3] = 'E';

    hdr.fmt_tag[0] = 'f';
    hdr.fmt_tag[1] = 'm';
    hdr.fmt_tag[2] = 't';
    hdr.fmt_tag[3] = ' ';

    hdr.data_tag[0] = 'd';
    hdr.data_tag[1] = 'a';
    hdr.data_tag[2] = 't';
    hdr.data_tag[3] = 'a';

    hdr.riff_len = rifflen;
    hdr.fmt_len = fmtlen;
    hdr.audio_format = audfmt;
    hdr.num_channels = chans;
    hdr.sample_rate = spf;
    hdr.byte_rate = spf * (bps/8);
    hdr.block_align = bps / 8;
    hdr.bits_per_sample = bps;
    hdr.data_len = 0;
    fwrite(&hdr, sizeof(hdr), 1, m_file);
    fflush(m_file);

}

}  // iz
