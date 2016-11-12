#include "plugin-manager.h"

#include <iostream>
#include <dlfcn.h>
#include <stdlib.h>

namespace iz {

// these are used for typecasting
typedef void (*init)(void);
typedef int (*put_ndata)(void*, int);
typedef int (*put_data)(void*);
typedef void* (*get_data)(void);
typedef void (*deinit)(void);

struct interface_t
{
    void (*init)(void);
    int (*put_ndata)(void*, int);
    int (*put_data)(void*);
    void* (*get_data)(void);
    void (*deinit)(void);
};

typedef struct interface_t* (*get_interface)();


QHash<QString, RecIface> RecPluginMngr::m_plugins;

/// TODO: this has to be changed to match
/// the correct name of the plugins
/// \brief RecPluginMngr::loadLibrary
/// \param src - must be a config file attribs
/// \return OK if loaded, fale else
///
bool RecPluginMngr::loadLibrary(const QString &src, const QString& name)
{
    bool res = false;
    QLibrary plugin(src);
    res = plugin.load();
    if (!res) {
        std::cout <<
                     plugin.errorString().toStdString()
                  << std::endl;
        return res;
    }

    bool load_all_res = false;
    RecIface iface;
    get_interface lib_ifaceCb = (get_interface) plugin.resolve("get_interface");

    if (lib_ifaceCb != nullptr) {
        struct interface_t* lib_symbols = lib_ifaceCb();
        if (lib_symbols->deinit == nullptr || lib_symbols->init == nullptr
                || lib_symbols->get_data == nullptr || lib_symbols->put_data == nullptr
                || lib_symbols->put_ndata == nullptr
              )
        {
            load_all_res = false;

        } else {
            iface.init = lib_symbols->init;
            iface.deinit = lib_symbols->deinit;
            iface.get_data = lib_symbols->get_data;
            iface.put_data = lib_symbols->put_data;
            iface.put_ndata = lib_symbols->put_ndata;
            load_all_res = true;
        }
        // old concept
        m_plugins[name] = iface;
        return load_all_res;

    } else {
        std::cout << plugin.errorString().toStdString()
                  << std::endl;
        return load_all_res;
    }
}

/// get a pointer to the iterface from the
/// map, by a given name:
/// ex. getInterface("Test!")->init();
/// \brief RecPluginMngr::getInterface
/// \param name of loaded plugin
/// \return
///
RecIface* RecPluginMngr::getInterface(const QString &iface)
{

    if( m_plugins.find(iface) != m_plugins.end() ) {
        return &m_plugins[iface];
    } else {
        return nullptr;
    }

}

RecPluginMngr::RecPluginMngr()
{

}

RecPluginMngr::~RecPluginMngr()
{

}

}
