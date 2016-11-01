#ifndef RECORDERCONFIG_H
#define RECORDERCONFIG_H

#include <QFile>
#include <QObject>
#include <QHash>

#include "types.h"

namespace iz {

// using in C++ as alternate to typedef, for now typedef
typedef QList<MPair<QString, QString> >  PairList;

class RecorderConfig : public QObject
{

public:
    static RecorderConfig& Instance();
    bool loadFile(const QString& fname);
    PairList& getTagPairs(const QString& tag);
private:
    explicit RecorderConfig(QObject* parent=nullptr);
    ~RecorderConfig();

    static RecorderConfig* s_inst;
    QString m_filename;
    // tag name, attributes
    QHash<QString, QList<MPair<QString, QString> > > m_tags;
};

}  // iz

#endif // RECORDERCONFIG_H
