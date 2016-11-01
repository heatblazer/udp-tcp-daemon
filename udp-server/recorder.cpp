#include "recorder.h"

namespace iz {

Recorder::Recorder(QObject *parent)
    : QObject(parent)
{

}

Recorder::~Recorder()
{
    // cleanup()
}

} // iz
