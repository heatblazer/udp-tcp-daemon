#include "wav-writer.h"
#include "defs.h"
namespace iz {

FILE* WavWriter::m_file = NULL;

bool WavWriter::load(const char *fname, const char *perms)
{
    if (!fname || !perms) {
        return false;
    }

    m_file = fopen(fname, perms);
    if (!m_file) {
        return false;
    } else {
        return true;
    }
}

void WavWriter::unload()
{
    if (m_file) {
        fclose(m_file);
    }
}

void WavWriter::writeRawData(void *data, int size, int offset)
{
    (void) data; (void) size; (void) offset;
}

void *WavWriter::readRawData()
{
    if (m_file) {
        size_t size = file_size();
        char* buff = new char[size+sizeof(WavHdr)];
        fgets(buff, size+sizeof(WavHdr), m_file);
        WavHdr* wav = (WavHdr*) buff;
        return buff;
    } else {
        return NULL;
    }

}

FILE *WavWriter::getFileHandle()
{

}

size_t WavWriter::file_size()
{
    size_t size = 0;
    if (m_file) {
        fseek(m_file, 0U, SEEK_END);
        size = ftell(m_file);
        fseek(m_file, 0U, SEEK_SET);
        return size;
    } else {
        return size;
    }
}

WavWriter::WavWriter()
{

}

WavWriter::~WavWriter()
{

}

}
