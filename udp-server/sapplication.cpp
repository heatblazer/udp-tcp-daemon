#include "sapplication.h"

#include <iostream>

namespace iz {


/// TODO: add arguments to the program
/// \brief SApplication::SApplication
/// \param argc
/// \param argv
///
SApplication::SApplication(int &argc, char **argv)
    : QCoreApplication(argc, argv),
      m_config("recorder.cfg")
{
}

SApplication::~SApplication()
{
}

int SApplication::init()
{
#if 0 // hinting
    for(int i=0; i < 32; ++i) {

        char fname[32]={0};
        sprintf(fname, "%d.wav", i);
        m_wavs[i] = new Wav(fname);
        m_wavs[i]->open("wb+");
        short data[88000]={0};
        int vol = 32000;
        for(int i=0; i < 88000; ++i) {
            data[i] = vol ;
        }

        m_wavs[i]->write(data, 88000);
        for(int i=0; i < 88000; ++i) {
            data[i] = vol/2 ;
        }

        m_wavs[i]->write(data, 88000);

        for(int i=0; i < 88000; ++i) {
            data[i] = vol/3 ;
        }

        m_wavs[i]->write(data, 88000);
        m_wavs[i]->close();
    }
#endif
    m_server.init();
    return 1;
}

} // iz
