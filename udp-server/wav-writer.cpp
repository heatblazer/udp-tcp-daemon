#include "wav-writer.h"
#include <math.h>
#include <string.h> // strcpy

#define WAVFILE_SAMPLES_PER_SECOND 44100

namespace iz {

Wav::Wav(const char *fname)
    : m_file(NULL)
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
    write_hdr();
    return true;
}

void Wav::close()
{
    int file_len = ftell(m_file);
    // get the len from the offset
    int data_len = file_len - sizeof(struct wav_hdr_t);
    // move the fp to that position
    fseek(m_file, sizeof(struct wav_hdr_t) - sizeof(int), SEEK_SET);
    fwrite(&data_len, sizeof(data_len), 1, m_file);

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

void Wav::write_hdr()
{
    struct wav_hdr_t hdr;
    int samples_per_second = WAVFILE_SAMPLES_PER_SECOND;
    int bits_per_sample = 16;
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

    hdr.riff_len = 0;
    hdr.fmt_len = 16;
    hdr.audio_format = 1;
    hdr.num_channels = 1;
    hdr.sample_rate = samples_per_second;
    hdr.byte_rate = samples_per_second * (bits_per_sample/8);
    hdr.block_align = bits_per_sample / 8;
    hdr.bits_per_sample = bits_per_sample;
    hdr.data_len = 0;

    fwrite(&hdr, sizeof(hdr), 1, m_file);
    fflush(m_file);
}

}  // iz
