#include "sapplication.h"

#include <iostream>
#include "unix/daemon.h"

namespace iz {


/// TODO: add arguments to the program
/// \brief SApplication::SApplication
/// \param argc
/// \param argv
///
SApplication::SApplication(int &argc, char **argv)
    : QCoreApplication(argc, argv),
      m_setup(false)
{
    // TODO: use other logic to specify xml file from argc/argv
    if (argc <= 1) {
        std::cout << "ERROR!\n"
                  << "program -c <configfile>\n"
                  << "program -d"
                  << std::endl;
        m_setup = false;
    } else {
        for(int i=0; i < argc; ++i) {
            if (argv[i] == QLatin1String("-c") ||
                    argv[i] == QLatin1String("--config")) {
                if (argv[i+1] != nullptr) {
                    // init config file
                    m_setup = true;
                    RecorderConfig::Instance().loadFile(QString(argv[i+1]));
                } else {
                    std::cout << "ERROR! Please provide a config file after ("
                              << argv[i] << ") argument! Terminating..."
                              << std::endl;
                    m_setup = false;
                }
            }
        }
    }
    iz::registerAppData(this);
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
    if (!m_setup) {
        return -1;
    } else {

        bool udp = false;
        quint16 port = 0;
        const MPair<QString, QString>& pair_dup =
                RecorderConfig::Instance().getAttribPairFromTag("Network", "udp");
        const MPair<QString, QString>& pair_port = RecorderConfig::Instance()
                .getAttribPairFromTag("Network", "port");
        bool parese_res = false;
        port = pair_port.m_type2.toInt(&parese_res);
        if (!parese_res) {
            port = 1234U;
        }

        if (pair_dup.m_type1 == "") {
            udp = false;
        } else {
            udp = true;
        }

        m_recorder.init();
        m_server.init(udp, port);
        // connect rec to server
        connect(&m_server, SIGNAL(dataReady(udp_data_t, uint32_t)),
                &m_recorder, SLOT(record(udp_data_t,uint32_t)));
    }
    return 0;
}

/// stop all stuff
void SApplication::deinit()
{
    m_recorder.deinit();
    m_server.deinit();
}

} // iz
