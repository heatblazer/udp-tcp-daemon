#ifndef RECORDERCONFIG_H
#define RECORDERCONFIG_H

#include <QFile>
#include <QObject>
#include <QHash>

#include "types.h"

namespace iz {

// C++ "using" keyword can replace typedefs
// but for now to prettyfu the code I`ll typedef
// this
typedef QList<MPair<QString, QString> >  PairList;

class RecorderConfig : public QObject
{

public:
    static RecorderConfig& Instance();
    bool loadFile(const QString& fname);
    void fastLoadFile(const QString& fname);
    PairList& getTagPairs(const QString& tag);
    const MPair<QString, QString> &getAttribPairFromTag(const QString &tag, const QString& attrib);
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
