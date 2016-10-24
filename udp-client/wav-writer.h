#ifndef WAVWRITER_H
#define WAVWRITER_H
#include <stdio.h>
namespace iz {

class WavWriter
{
public:
    static bool load(const char* fname, const char* perms);
    static void unload();
    static void writeRawData(void* data, int size, int offset);
    static void* readRawData();
    static FILE*  getFileHandle();
private:
    static size_t file_size();
    WavWriter();
    ~WavWriter();

    static FILE* m_file;
};

}


#endif // WAVWRITER_H
