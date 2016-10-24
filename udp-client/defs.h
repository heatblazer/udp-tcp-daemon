#ifndef DEFS_H
#define DEFS_H

#include <stdint.h>
namespace iz {

struct WavHdr
{
    char riff[4];
    int32_t file_size;
    char wave[4];
    char fmt[4];
    int16_t len;
    int16_t type;
    uint16_t format;
    int32_t sample_rate;
    int32_t sr_bs_channs;
    uint8_t bits_per_sample;
    char data[4];
    int32_t fsize;
};

}
#endif // DEFS_H
