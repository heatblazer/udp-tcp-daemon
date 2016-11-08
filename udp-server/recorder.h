#ifndef RECORDER_H
#define RECORDER_H

// qt - paretn //
#include <QFileSystemWatcher> // monitor wav files for size for the hotswap
#include <QObject>
#include <QTimer> // hotswap interval

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
    // will use later the num_channels when concept is more clear
    bool init(int hot_swap=10000 /*,int num_chans=32*/);
    void deinit();


private:
    bool setupWavFiles();

signals:
    void recordedBytes(uint32_t bytes);

public slots:
    void record(const udp_data_t& data);
    void record(const tcp_data_t& data);

private slots:
    // unimplemented yest
    void hotSwapFiles();
    // remove after  test
    void testFileWatcher(const QString& file);

private:
    // 32 chans for now
    // but will be configured outside
    Wav* m_wavs[32];
    QTimer m_hotswap;
    QFileSystemWatcher m_filewatcher;
    uint32_t    m_maxFileSize;
};

} // iz

#endif // RECORDER_H
