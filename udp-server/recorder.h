#ifndef RECORDER_H
#define RECORDER_H

// qt - paretn //
#include <QObject>

namespace iz {

class Recorder : public QObject
{
    Q_OBJECT // this class may be emiter
public:
    explicit Recorder(QObject* parent=nullptr);
    virtual ~Recorder(); // we may inherit it too

private:

};

} // iz

#endif // RECORDER_H
