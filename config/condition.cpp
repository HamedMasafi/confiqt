#include "condition.h"
#include "configmanager.h"

#include <QDebug>
#include <QRegularExpression>

QString Condition::cond() const
{
    return _cond;
}

void Condition::setCond(const QString &cond)
{
    _cond = cond;
}

Condition::Condition() : d(new ConditionData)
{

}

Condition::Condition(const Condition &other) : d(other.d)
{
}

Condition::Condition(const QString &cond, const ConfigManager *config)
    : d(new ConditionData), _cond(cond), _config(config)
{
}

Condition Condition::operator =(const Condition &other)
{
    return Condition(other);
}

Condition Condition::operator =(Condition &&other)
{
    return Condition(other);
}

bool Condition::check()
{
    if (_cond.isEmpty())
        return true;
    QString cond(_cond);
    QRegularExpression r("\\w+\\.\\w+");
     QRegularExpressionMatchIterator i = r.globalMatch(cond);
     while (i.hasNext()) {
         QRegularExpressionMatch m = i.next();
         auto parts = m.captured(0).split(".");
         if (parts.length() != 2)
             continue;

         QString val = "";
         if (parts[0] == "features") {
             auto state = _config->featureState(parts[1]);

             if (state == Qt::Unchecked)
                 val = "0";
             else
                 val = "1";
             d->results.insert("Feature " + parts[1], val == "1");
         }
         if (parts[0] == "config") {
             val = _config->hasConfig(parts[1]) ? "1" : "0";
             d->results.insert("Config " + parts[1], val == "1");
         }
         if (parts[0] == "test") {
             val = "1";
         }
         cond  = cond.replace(m.captured(0), val);

     }

//     cond.clear();
//     for (int i = 0; i < _cond.length(); ++i) {
//         auto ch = _cond.at(i);
////         if (ch.isLetter() || ch == ".")
//     }
     QString cond2 = cond.replace(" ", "");
     forever{
        cond = cond
                .replace("1&&1", "1")
                .replace("1&&0", "0")
                .replace("0&&1", "0")
                .replace("0&&0", "0")

                .replace("1||1", "1")
                .replace("1||0", "1")
                .replace("0||1", "1")
                .replace("1||0", "0")

                .replace("(1)", "1")
                .replace("(0)", "0")

                .replace("!1", "0")
                .replace("!0", "1");
         if (cond2 == cond)
             break;
         cond2 = cond;
     }
     qDebug() << cond;
     d->finalResult = cond == "1";
     return d->finalResult;
}

QString Condition::toString() const
{
    QString s;

    auto i = d->results.begin();
    while (i != d->results.end()) {
        if (!s.isEmpty())
            s.append("\n");
        s.append(QString("%1: %2")
                 .arg(i.key())
                 .arg(i.value() ? "true" : "false"));
        ++i;
    }
    return s;
}
