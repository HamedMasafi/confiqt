#include "conditiondata.h"



ConditionData::ConditionData() : _config(nullptr)
{

}

ConditionData::ConditionData(const QString &cond, const ConfigManager *config)
    : _cond(cond), _config(config)
{

}
