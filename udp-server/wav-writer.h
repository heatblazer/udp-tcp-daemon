#ifndef WAVWRITER_H
#define WAVWRITER_H

// qt //
#include <QFile>
#include <QDataStream>
#include <QThread>
#include <QMutex>

// ansi c //
#include <stdio.h>
#include <stdint.h>

// defs //
#include "defs.h"

namespace iz {

class Wav
{

public:
    Wav(const char* fname);
    ~Wav();
    // in case we migrate to other file api we made these virtual
    // but provide some defailt implementation
    virtual bool open(const char* perms);
    virtual void close();
    virtual int write(short int data[], int len);

private:
    void write_hdr(int spf=44100, int bps=16, int rifflen=0, int fmtlen=16, short audfmt=1, short chans=1);

private:
    FILE*   m_file;
    char m_filename[64];
};


} // iz


#endif // WAVWRITER_H
