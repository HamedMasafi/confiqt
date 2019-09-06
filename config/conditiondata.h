#ifndef CONDITIONDATA_H
#define CONDITIONDATA_H

#include <QMap>
#include <QSharedData>

class ConditionData : public QSharedData {
public:
    bool finalResult;
    QMap<QString, bool> results;
};

#endif // CONDITIONDATA_H
