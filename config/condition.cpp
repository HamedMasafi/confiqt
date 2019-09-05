#include "condition.h"
#include "configmanager.h"

#include <QDebug>
#include <QRegularExpression>

Condition::Condition(const QString &cond, const ConfigManager *config)
    : _cond(cond), _config(config)
{
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
         }
         if (parts[0] == "config") {
             val = "1";
         }
         cond  = cond.replace(m.captured(0), val);

     }

//     cond.clear();
     for (int i = 0; i < _cond.length(); ++i) {
         auto ch = _cond.at(i);
//         if (ch.isLetter() || ch == ".")
     }
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
     return cond == "1";
}
