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

    bool load_all_res = true;
    RecIface iface;
    iface.init = (init) plugin.resolve("init");
    // todo: err messages !!!

    if (iface.init == nullptr) {
        load_all_res &= false;
    }

    iface.get_data = (get_data) plugin.resolve("get_data");
    if (iface.get_data == nullptr) {
        load_all_res &= false;
    }

    iface.put_data = (put_data) plugin.resolve("put_data");
    if (iface.put_data == nullptr) {
        load_all_res &= false;
    }

    iface.put_ndata = (put_ndata) plugin.resolve("put_ndata");
    if (iface.put_ndata == nullptr) {
        load_all_res &= false;
    }
    // map them all
    m_plugins[name] = iface;
    return load_all_res;


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
