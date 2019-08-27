#ifndef GLOBAL_H
#define GLOBAL_H

#include <QDebug>
#include <QString>
#include <QVariantMap>

struct Option {
    QString name;
    QString type;
    QVariant values;
    QString moduleName;

    QVariant::Type variantType() const {
        if (type == "boolean")
            return QVariant::Bool;

        if (type == "enum")
            return QVariant::List;

        if (type == "optionalString" || type == "string")
            return QVariant::String;

        if (type == "void")
            return QVariant::Invalid;

        return QVariant::String;
    }
    QVariant createValues() {
        if (type == "enum") {
            QVariantList list;
            if (values.type() == QVariant::Map) {
                auto keys = values.toMap().keys();
                foreach(QString k, keys)
                    list.append(k);
            }
            if (values.type() == QVariant::List) {
                list = values.toList();
            }
            return list;
        }

        return QVariant();
    }
};

struct Feature {
    QString name;
    QString label;
    QString purpose;
    QString section;
    QString moduleName;
    QStringList condition;
};
Q_DECLARE_METATYPE(Feature);

enum Roles {
    CheckStateRole = Qt::UserRole,
    DropDownRole,
    TypeRole,
    DataRole
};

enum class FeatureTreeNodeType {
    Module,
    Section,
    Feature
};

class Global
{
public:
    Global();
};

#endif // GLOBAL_H
