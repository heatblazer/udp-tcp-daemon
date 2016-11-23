#include "sapplication.h"

#include <iostream> // remove it later

// qt stuff//
#include <QDir>

// local headers //
#include "utils/logger.h"
#include "unix/daemon.h"

namespace iz {


int SApplication::m_fdHUP = -1;
int SApplication::m_fdTERM = -1;
int SApplication::s_argc = 0;
char** SApplication::s_argv = 0;

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
    s_argc = argc;
    s_argv = argv;

    // just program name
    if (argc == 1) {
        // initializing with default setting
        std::cout << "Warning! You are not using a config file! Loading defaults!"
                  << std::endl;
        m_setup = RecorderConfig::Instance().loadDefaults();
    } else {
        // wakt the args
        for(int i=0; i < argc; ++i) {
            if (strcmp(argv[i], "-c") == 0 ||
                strcmp(argv[i], "--config") == 0) {
                if (argv[i+1] != nullptr) {
                    // init config file

                  // I will test the fast loading of XML file here
#ifndef UNSAFE_CONFIG
                    // this will be safe check element by element and attribs
                    // and will return true or false
                    m_setup = RecorderConfig::Instance().loadFile(QString(argv[i+1]));
#else
                    // this is lab tested xml parsing assuming
                    // config is OK, no checks and always return TRUE
                    // which is dangerous in release
                    m_setup = RecorderConfig::Instance().fastLoadFile(QString(argv[i+1]));
                    if (!m_setup) {
                        std::cout << "Warning! You did not point a config file after: ("
                                  << argv[i] << ") argument! Loading application with defaults!"
                                  << std::endl;
                        m_setup = RecorderConfig::Instance().loadDefaults();
                    }
#endif
                }
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
    bool log_init = Logger::Instance().init();
    if (!log_init) {
        exit(11);
    }

    Logger::Instance().logMessage("Initializing application...\n");
    if (!m_setup) {
        Logger::Instance().logMessage("Failed to initialize application!\n");
        return -1;
    } else {
        // transport initialization
        bool udp = false;
        quint16 port = 0;
        const MPair<QString, QString>& trans_attr =
                RecorderConfig::Instance().getAttribPairFromTag("Network", "transport");

        const MPair<QString, QString>& port_attr = RecorderConfig::Instance()
                .getAttribPairFromTag("Network", "port");

        bool parese_res = false;
        port = port_attr.m_type2.toInt(&parese_res);
        if (!parese_res) {
            char msg[128] = {0};
            snprintf(msg, sizeof(msg), "Parsing (%s) failed. Will use default: (%d)\n",
                    port_attr.m_type2.toStdString().data(),
                    1234);
            Logger::Instance().logMessage(msg);
            port = 1234U;
        }

        if (trans_attr.m_type2 == "udp") {
            udp = true;
        } else {
            udp = false;
        }
        // they need not to depend each other
        if (!m_recorder.init()) {
            Logger::Instance().logMessage("Failed initialize recorder\n");
        }

#ifdef HEARTATTACK
        bool attack = true;
#else
        bool attack = false;
#endif
        // this is for test purpose only! Remove later!!!
        m_server.init(udp, port, attack);

        // connect rec to server
        if (udp) {
            connect(&m_server, SIGNAL(dataReady(udp_data_t)),
                &m_recorder, SLOT(record(udp_data_t)));
        } else {
            connect(&m_server, SIGNAL(dataReady(tcp_data_t)),
                    &m_recorder, SLOT(record(tcp_data_t)));
        }
    }

    // I`ll better call that in the constructor
    // since I may want to pass some args to
    // the main proxies
    // for now nothing...
    Logger::Instance().logMessage("Loading pluggins...\n");
    loadPlugins();
     // test loaded plugins
    testLoadedPlugins();

    Logger::Instance().logMessage("Initialization of application completed!\n");

    m_user_server.setObjectName("user server");
    m_user_server.moveToThread(&m_user_server);
    m_user_server.start();

    //std::cout << GITHASH << std::endl;
    return 0;
}

/// stop all stuff
void SApplication::deinit()
{
    Daemon::log("SApplication::deinit()!\n");
    m_recorder.deinit();
    m_server.deinit();
    for(int i=0; i < m_plugins.count(); ++i) {
        // deinit in priority order
        m_plugins.at(i).deinit();
    }

    m_user_server.wait(1000);
}

void SApplication::loadPlugins()
{
    // plugin setup section
    // this is a bit toug logic for now
    PairList list = RecorderConfig::Instance().getTagPairs("Plugin");
    char msg[128] = {0};

    for(int i=0; i < list.count(); ++i) {
       if (list.at(i).m_type1 == "name" && list.at(i).m_type2 != "") {
            // perform the parsina and plugin setup here
            // the array is ordered and we assume name is
            // in the front
           snprintf(msg, sizeof(msg), "Loading (%s) plugin...\n",
                   list.at(i).m_type2.toStdString().data());
           Logger::Instance().logMessage(msg);

           RecPluginMngr::loadLibrary(list.at(i+3).m_type2, list.at(i).m_type2);
           RecIface* iface =
                    RecPluginMngr::getInterface(list.at(i).m_type2);
           // put in any order for now
           // store into the indexed array
           if (iface != nullptr) {
               snprintf(msg, sizeof(msg), "Loaded (%s) plugin.\n", list.at(i).m_type2.toStdString().data());
               Logger::Instance().logMessage(msg);
               m_plugins.push_back(*iface);
           } else {
               snprintf(msg, sizeof(msg), "\nFailed to load (%s) plugin.\n",
                       list.at(i).m_type2.toStdString().data());
                Logger::Instance().logMessage(msg);
           }
        }
    }
}

/// TODO: remove this function later
/// \brief SApplication::testLoadedPlugins
///
void SApplication::testLoadedPlugins()
{
    std::cout << "Testing loaded plugins!" << std::endl;
    for(int i=0; i < m_plugins.count(); ++i) {
        // prevend segfaults
        if (m_plugins.at(i).init) {
            m_plugins.at(i).init();
        }
        if (m_plugins.at(i).get_data()) {
            m_plugins.at(i).get_data();
        }
        if (m_plugins.at(i).put_data) {
            m_plugins.at(i).put_data(0);
        }
        if (m_plugins.at(i).put_ndata) {
            m_plugins.at(i).put_ndata(0, 0);
        }
        if (m_plugins.at(i).main_proxy) {
            m_plugins.at(i).main_proxy(SApplication::s_argc,
                                       SApplication::s_argv);
        }
    }
}

/// TODO: remove after tests
/// \brief SApplication::testKill
///
void SApplication::testKill()
{
    Daemon::sendSignal(Daemon::m_pid, SIGKILL);
}

} // iz
