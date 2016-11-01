#include "recorder.h"

// wav library //
#include "wav-writer.h"

namespace iz {

Recorder::Recorder(QObject *parent)
    : QObject(parent)
{

}

Recorder::~Recorder()
{
    // cleanup()
}

void Recorder::record(const udp_data_t &data, uint32_t slot)
{
    if (slot > 31) {
        return; // out of max chan index
    }
    uint32_t data_size = (sizeof(data) -
            (sizeof(data.counter) + (sizeof(data.null_bytes)/sizeof(data.null_bytes[0])))
            );

    m_wavs[slot]->write((short*)data.data, data_size);
}

} // iz
