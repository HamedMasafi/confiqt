#include "jsonconfig.h"

#include <QFile>
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QVariant>

JsonConfig::JsonConfig(const QString &filePath, const QString &moduleName, QObject *parent)
    : QObject(parent), _filePath(filePath), _moduleName(moduleName)
{
    QString p(_filePath);

    if (!p.endsWith("/configure.json"))
        p.append("/configure.json");

    QFile jsonFile(p);
    if (!jsonFile.open(QIODevice::Text | QIODevice::ReadOnly))
        return;

    QByteArray buffer = jsonFile.readAll();
    jsonFile.close();

    if (buffer == QByteArray())
        return;

    QJsonDocument doc = QJsonDocument::fromJson(buffer);

    if (doc.isObject())
        _obj = doc.object();
}

bool JsonConfig::isValid() const
{
    return _obj != QJsonObject();
}

QStringList JsonConfig::subConfigs() const
{
    if (_obj.value("subconfigs").isArray()) {
        QJsonArray arr = _obj.value("subconfigs").toArray();
        QStringList list;
        for (int i = 0; i < arr.size(); ++i)
            list.append(arr.at(i).toString());
        return list;
    }
    return QStringList();
}

QList<Option> JsonConfig::options() const
{
    QJsonObject cmdline = _obj.value("commandline").toObject();
    if (cmdline == QJsonObject())
        return QList<Option>();

    QJsonObject options = cmdline.value("options").toObject();
    if (options == QJsonObject())
        return QList<Option>();

    QList<Option> ret;
    foreach (QString key, options.keys()) {
        QJsonValue v = options.value(key);
        Option o;
        o.name = key;
        o.moduleName = _moduleName;
        if (v.isString())
            o.type = v.toString();
        else if (v.isObject()) {
            QJsonObject obj = v.toObject();
            o.type = obj.value("type").toString();

            QVariant vv = obj.value("values").toVariant();
            o.values = vv;
        }
        ret.append(o);
    }
    return ret;
}

QList<Feature> JsonConfig::features() const
{
    QJsonObject f = _obj.value("features").toObject();
    if (f == QJsonObject())
        return QList<Feature>();

    QList<Feature> ret;
    foreach (QString key, f.keys()) {
        Feature ft;
        ft.name = key;
        QJsonObject obj = f.value(key).toObject();

        QJsonValue label = obj.value("label");
        if (label.isString())
            ft.label = label.toString();

        ft.purpose = obj.value("purpose").toString();
        ft.section = obj.value("section").toString();
        ft.moduleName = _moduleName;

        auto condition = obj.value("condition");
        if (condition.isString())
            ft.condition = QStringList() << condition.toString();
        else if (condition.isArray()) {
            auto vl = condition.toArray().toVariantList();
            foreach (QVariant v, vl)
                ft.condition.append(v.toString());
        }
        ret.append(ft);
    }
    return ret;
}
