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

public:
    Condition();
    Condition(const Condition &other);
    Condition(const QString &cond, const ConfigManager *config);
    Condition &operator =(const Condition &other);
    Condition &operator =(Condition &&other);
    bool check();
    QString toString() const;
    QString cond() const;
    void setCond(const QString &cond);
    QStringList requirement() const;
    bool result() const;
};

#endif // CONDITION_H
