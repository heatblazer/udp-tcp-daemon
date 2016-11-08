#ifndef WAVWRITERIFACE_H
#define WAVWRITERIFACE_H

namespace iz {

class WavIface
{
public:
    virtual bool open(const char* perms) = 0;
    virtual void close() = 0;
    virtual int write(short int data[], int len) = 0;
    virtual void setupWave(int samples_per_sec, int bits_per_sec, int riff_len,
                            int fmt_len, short audio_fmt,  short chann_cnt) = 0;
    virtual void* read() = 0;
    virtual bool isOpened() const = 0;

};

}

#endif // WAVWRITERIFACE_H
