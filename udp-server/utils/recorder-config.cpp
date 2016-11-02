#include "recorder-config.h"

// remove me
#include <iostream>

#include <QXmlStreamReader>

namespace iz {

RecorderConfig* RecorderConfig::s_inst = nullptr;

RecorderConfig::RecorderConfig(QObject *parent)
    : QObject(parent)
{

}

RecorderConfig::~RecorderConfig()
{
}

bool RecorderConfig::loadFile(const QString &fname)
{
    QFile file(fname);
    if (file.open(QIODevice::ReadOnly)) {
        QXmlStreamReader reader(file.readAll());
        file.close();
        while(!reader.atEnd()) {
            reader.readNext();
            if (reader.isStartElement()) {
                // rework the logic here
                if (reader.name() == "HotSwap") {
                    QXmlStreamAttributes attribs = reader.attributes();
                    for(int i=0; i < attribs.count(); ++i) {
                        m_tags["HotSwap"].append(MPair<QString, QString>(
                                                     attribs.at(i).name().toString(),
                                                      attribs.at(i).value().toString()));
                    }
                } else if (reader.name() == "Channels") {
                    QXmlStreamAttributes attribs = reader.attributes();
                    for(int i=0; i < attribs.count(); ++i) {
                        m_tags["Channels"].append(MPair<QString, QString>
                                                  (attribs.at(i).name().toString(),
                                                       attribs.at(i).value().toString()));
                    }

                } else if (reader.name() == "Wave") {
                    QXmlStreamAttributes attribs = reader.attributes();
                    for(int i=0; i < attribs.count(); ++i) {
                        m_tags["Wave"].append(MPair<QString, QString>
                                              (attribs.at(i).name().toString(),
                                                       attribs.at(i).value().toString()));

                    }

                } else if (reader.name() == "Record"){
                    QXmlStreamAttributes attribs = reader.attributes();
                    for(int i=0; i < attribs.count(); ++i) {
                        m_tags["Record"].append(MPair<QString, QString>
                                                (attribs.at(i).name().toString(),
                                                       attribs.at(i).value().toString()));
                    }
                } else if (reader.name() == "Network"){
                    QXmlStreamAttributes attribs = reader.attributes();
                    for(int i=0; i < attribs.count(); ++i) {
                        m_tags["Network"].append(MPair<QString, QString>
                                                (attribs.at(i).name().toString(),
                                                       attribs.at(i).value().toString()));
                    }
                } else if (reader.name() == "Log") {
                    QXmlStreamAttributes attribs = reader.attributes();
                    for(int i=0; i < attribs.count(); ++i) {
                        m_tags["Log"].append(MPair<QString, QString>
                                                (attribs.at(i).name().toString(),
                                                       attribs.at(i).value().toString()));
                    }
                } else if (reader.name() == "Heartbeat") {
                    QXmlStreamAttributes attribs = reader.attributes();
                    for(int i=0; i < attribs.count(); ++i) {
                        m_tags["Heartbeat"].append(MPair<QString, QString>
                                                (attribs.at(i).name().toString(),
                                                       attribs.at(i).value().toString()));
                    }
                } else {
                    // dummy else for now
                    // later will support
                    // default config
                }
            }
        }
    }
    // safety check later !!!
    return true;
}

/// this is fast, non precision, no checking
/// function to just load all tags and attribs
/// do not use in release, jsut in tests
/// \brief RecorderConfig::fastLoadFile
/// \param fname
///
void RecorderConfig::fastLoadFile(const QString &fname)
{
    QFile file(fname);
    if (file.open(QIODevice::ReadOnly)) {
        QXmlStreamReader reader(file.readAll());
        file.close();
        while(!reader.atEnd()) {
            reader.readNext();
            if (reader.isStartElement()) {
                QXmlStreamAttributes attribs = reader.attributes();
                for(int i=0; i < attribs.count(); ++i) {
                    m_tags[reader.name().toString()]
                            .append(MPair<QString, QString>
                                              (attribs.at(i).name().toString(),
                                               attribs.at(i).value().toString()));
                }
            }
        }
    }
}

/// no check for now
/// \brief RecorderConfig::getTagPairs
/// \param tag - tag name
/// \return  list of attributes
///
PairList &RecorderConfig::getTagPairs(const QString &tag)
{
    return m_tags[tag];
}

const MPair<QString, QString>
&RecorderConfig::getAttribPairFromTag(const QString &tag, const QString& attrib)
{
    PairList& l = getTagPairs(tag);
    for(int i=0; i < l.count(); ++i) {
        if (l.at(i).m_type1.compare(attrib)) {
            return l.at(i);
        }
    }
    // return some error static object
    static MPair<QString, QString> none("", "");
    return none;
}

RecorderConfig &RecorderConfig::Instance()
{
    if (s_inst == nullptr) {
        s_inst = new RecorderConfig;
    }
    return *s_inst;
}


} // iz
