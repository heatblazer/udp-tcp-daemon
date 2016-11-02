#ifndef RECORDER_H
#define RECORDER_H

// qt - paretn //
#include <QObject>

#include "types.h"
#include "utils/recorder-config.h"

namespace iz {

class Wav;

class Recorder : public QObject
{
    Q_OBJECT // this class may be emiter
public:
    explicit Recorder(QObject* parent=nullptr);
    virtual ~Recorder(); // we may inherit it too
    bool init(void);
    void deinit();
    bool setupWavFiles();

signals:
    void recordedBytes(uint32_t bytes);

public slots:
    void record(const udp_data_t& data, uint32_t slot);

private:
    // 32 chans for now
    // but will be configured outside
    Wav* m_wavs[32];
};

} // iz

#endif // RECORDER_H
