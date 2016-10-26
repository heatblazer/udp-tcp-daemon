#include "sapplication.h"

#include <iostream>

namespace iz {


/// TODO: add arguments to the program
/// \brief SApplication::SApplication
/// \param argc
/// \param argv
///
SApplication::SApplication(int &argc, char **argv)
    : QCoreApplication(argc, argv)
{
    for(int i=0; i < 32; ++i) {
        m_wavs[i] = nullptr;
    }
}

SApplication::~SApplication()
{
    for(int i=0; i < 31; ++i) {
        m_wavs[i]->close();
    }
}

int SApplication::init()
{
    for(int i=0; i < 32; ++i) {

        char fname[32]={0};
        sprintf(fname, "%d.wav", i);
        m_wavs[i] = new Wav(fname);
        m_wavs[i]->open("wb+");
        short data[88000]={0};
        int freq = 440;
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
    // attach test sig handler to all sigs
    m_server.init();
    return 1;
}

} // iz
