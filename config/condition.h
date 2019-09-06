#ifndef CONDITION_H
#define CONDITION_H

#include "conditiondata.h"

#include <QExplicitlySharedDataPointer>
#include <QString>

class ConditionData;
class ConfigManager;
class Condition
{
    QExplicitlySharedDataPointer<ConditionData> d;
    QString _cond;
    const ConfigManager *_config;

public:
    Condition();
    Condition(const Condition &other);
    Condition(const QString &cond, const ConfigManager *config);
    Condition operator =(const Condition &other);
    Condition operator =(Condition &&other);
    bool check();
    QString toString() const;
    QString cond() const;
    void setCond(const QString &cond);
};

#endif // CONDITION_H
