#ifndef RECORDERCONFIG_H
#define RECORDERCONFIG_H

#include <QFile>
#include <QObject>

namespace iz {

class RecorderConfig : public QObject
{
public:
    explicit RecorderConfig(QObject* parent=nullptr);
    virtual ~RecorderConfig();
    bool loadFile(const QString& fname);
    QString& getAttribute(const QString& key);

private:
    QString m_filename;
};

}  // iz

#endif // RECORDERCONFIG_H
