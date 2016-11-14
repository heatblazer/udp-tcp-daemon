#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QObject>
#include <QLibrary>
#include <QHash>

#include "types.h"

namespace iz {


// plugin manager shall load a library from
// .so or .dll file then resolves the deps
class RecPluginMngr
{
public:
    static bool loadLibrary(const QString& src, const QString& name);
    static RecIface* getInterface(const QString& iface);
    static void unloadLibrary(const QString& lib);

private:
    RecPluginMngr();
    ~RecPluginMngr();
    void resolve();
    static QHash<QString, RecIface> m_plugins;
};

}

#endif // PLUGINMANAGER_H
