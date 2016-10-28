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
    m_server.init();
    return 1;
}

} // iz
