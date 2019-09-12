#ifndef JSONCONFIG_H
#define JSONCONFIG_H

#include "global.h"

#include <QJsonObject>
#include <QObject>

class Option;
struct Feature;
class JsonConfig : public QObject
{
    Q_OBJECT
    QString _filePath;
    QJsonObject _obj;
    QString _moduleName;

public:
    explicit JsonConfig(const QString &filePath, const QString &moduleName, QObject *parent = nullptr);

    bool isValid() const;
    QStringList subConfigs() const;
    QList<Option*> options() const;
    QList<Feature*> features() const;

signals:

public slots:
};

#endif // JSONCONFIG_H
