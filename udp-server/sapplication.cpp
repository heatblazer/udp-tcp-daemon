#include "sapplication.h"

#include <iostream>
#include "unix/daemon.h"

namespace iz {


int SApplication::m_fdHUP = -1;
int SApplication::m_fdTERM = -1;

void SApplication::writeToSocket(const char *data)
{
    (void) data;
}

/// the inheritee of QApplication is
/// responsible for initing all stuff
/// \brief SApplication::SApplication
/// \param argc
/// \param argv
///
SApplication::SApplication(int &argc, char **argv)
    : QCoreApplication(argc, argv),
      m_setup(false)
{
    // TODO: use other logic to specify xml file from argc/argv
        // get opts from args and find the xml file
    for(int i=0; i < argc; ++i) {
        if (argv[i] == QLatin1String("-c") ||
                argv[i] == QLatin1String("--config")) {
            if (argv[i+1] != nullptr) {
                // init config file
                m_setup = RecorderConfig::Instance().loadFile(QString(argv[i+1]));
            } else {
                std::cout << "ERROR! Please provide a config file after ("
                          << argv[i] << ") argument! Terminating..."
                          << std::endl;
                m_setup = false;
            }
        }
    }

    // old version:
    // daemon registration of this app to be used in the
    // future signal handlers and do some stuff there
    // I`ve writen a note why a static global instance is needed
    // for this, since sigactions calbback`s user data does not
    // refer to a concrete stuff and can`t be casted to what
    // we need.
    // new version:
    // will use socket pair to connect damon to the sapplication
    // and interprocess comunicate between it
    if (m_setup) {
        Daemon::registerAppData(this);
    }

}

SApplication::~SApplication()
{
    // if something bad happens, the dtor won`t be called
    // in daemon logic we have a handler to do the deinit procedure
}

/// init all modules that we`ll need here
/// \brief SApplication::init
/// \return -1 if something is not OK in the initialization
///
int SApplication::init()
{
    if (!m_setup) {
        return -1;
    } else {

        bool udp = false;
        quint16 port = 0;
        const MPair<QString, QString>& trans_attr =
                RecorderConfig::Instance().getAttribPairFromTag("Network", "transport");
        const MPair<QString, QString>& port_attr = RecorderConfig::Instance()
                .getAttribPairFromTag("Network", "port");
        bool parese_res = false;
        port = trans_attr.m_type2.toInt(&parese_res);
        if (!parese_res) {
            port = 1234U;
        }

        if (port_attr.m_type2 == "udp") {
            udp = true;
        } else {
            udp = false;
        }
        // they need not to depend each other
        m_recorder.init();
        m_server.init(udp, port);
        // connect rec to server
        if (udp) {
            connect(&m_server, SIGNAL(dataReady(udp_data_t)),
                &m_recorder, SLOT(record(udp_data_t)));
        } else {
            connect(&m_server, SIGNAL(dataReady(tcp_data_t)),
                    &m_recorder, SLOT(record(tcp_data_t)));
        }
    }

#if 0
    static QTimer quit;
    quit.setInterval(20000);
    connect(&quit, SIGNAL(timeout()),
            this, SLOT(testKill()));
    quit.start();
#endif
    return 0;
}

/// stop all stuff
void SApplication::deinit()
{
    Daemon::log("SApplication::deinit()!\n");
    m_recorder.deinit();
    m_server.deinit();
}

void SApplication::testKill()
{
    Daemon::sendSignal(Daemon::m_pid, SIGKILL);
}

} // iz
