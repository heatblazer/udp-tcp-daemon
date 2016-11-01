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
                std::cout << reader.name().toString().toStdString() << std::endl;
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
                        std::cout << attribs.at(i).name().toString().toStdString() << std::endl;
                        std::cout << attribs.at(i).value().toString().toStdString() << std::endl;
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

/// no check for now
/// \brief RecorderConfig::getTagPairs
/// \param tag - tag name
/// \return  list of attributes
///
QList<MPair<QString, QString> > &RecorderConfig::getTagPairs(const QString &tag)
{
    return m_tags[tag];
}

RecorderConfig &RecorderConfig::Instance()
{
    if (s_inst == nullptr) {
        s_inst = new RecorderConfig;
    }
    return *s_inst;
}


} // iz
