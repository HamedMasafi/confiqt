#ifndef CONDITIONDATA_H
#define CONDITIONDATA_H

#include <QMap>
#include <QSharedData>

class ConfigManager;
class ConditionData : public QSharedData {
public:
    ConditionData();
    ConditionData(const QString &cond, const ConfigManager *config);

    QString _cond;
    const ConfigManager *_config;
    bool finalResult;
    QMap<QString, bool> results;
    QStringList requirement;
};

#endif // CONDITIONDATA_H
