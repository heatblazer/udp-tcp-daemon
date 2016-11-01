#include "recorder-config.h"

// remove me
#include <iostream>

#include <QXmlStreamReader>

namespace iz {

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
                    int dbg =0;
                } else if (reader.name() == "Channels") {
                    QXmlStreamAttributes attribs = reader.attributes();
                    int dbg =0;
                } else if (reader.name() == "Wave") {
                    QXmlStreamAttributes attribs = reader.attributes();
                    int dbg =0;
                } else {

                }
            }
        }
    }
}

QString &RecorderConfig::getAttribute(const QString &key)
{

}

} // iz
