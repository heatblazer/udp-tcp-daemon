#ifndef CONFIG_H
#define CONFIG_H
// qt stuff //
#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

namespace iz {

// simple cfg file reader for now
class RecConfig
{
    //int spf, int bps, int rifflen, int fmtlen, short audfmt, short chans
public:
    enum Params {
        SAMPLES_PER_FRAME,
        BITS_PER_SEC,
        AUD_FORMAT,
        CHANNELS,
        SIZE
    };

    explicit RecConfig(const QString& fname);
    virtual ~RecConfig();
    int getAttribute(Params atrib);

private:
    struct {
        QString name;
        QFile   file;
    } m_file;
    int m_attribs[Params::SIZE];
};

}  // iz


#endif // CONFIG_H
