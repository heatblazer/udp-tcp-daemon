#ifndef QWAVEWRITER_H
#define QWAVEWRITER_H

// qt stuff //
#include <QFile>

#include "wav-writer.h"

namespace iz {
/// newer concept for wave writer using QT
/// \brief The QWave class
///
class QWave : public Wav
{

private:
    QFile m_file;
};


} // iz


#endif // QWAVEWRITER_H
