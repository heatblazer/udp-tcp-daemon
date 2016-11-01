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
    // TODO: use other logic to specify xml file
}

SApplication::~SApplication()
{
}

/// init all modules that we`ll need here
/// \brief SApplication::init
/// \return
///
int SApplication::init()
{
    m_recorder.init();
    m_server.init();
    // connect rec to server
    connect(&m_server, SIGNAL(dataReady(udp_data_t)),
            &m_recorder, SLOT(record(udp_data_t,uint32_t)));
    return 1;
}

} // iz
