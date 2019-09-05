#ifndef CONDITION_H
#define CONDITION_H

#include <QString>



class ConfigManager;
class Condition
{
    QString _cond;
    const ConfigManager *_config;

public:
    Condition(const QString &cond, const ConfigManager *config);
    bool check();
};

#endif // CONDITION_H
