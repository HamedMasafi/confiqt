#ifndef GLOBAL_H
#define GLOBAL_H

#include <QDebug>
#include <QString>
#include <QVariantMap>

struct Option {
    enum Type {
        Unknown,
        Bool,
        Enum,
        String,
        OptionalString,
        AddString,
        Void
    };
    QString name;
    Type type;
    QString typeString;
    QVariant values;
    QString moduleName;
    QString filePath;

    void setType(const QString &typeString) {
        type = Unknown;
        if (typeString == "boolean")
            type = Bool;

        if (typeString == "enum")
            type = Enum;

        if (typeString == "string")
            type = String;

        if (typeString == "optionalString")
            type = OptionalString;

        if (typeString == "addString")
            type = AddString;

        if (typeString == "void")
            type =  Void;

        this->typeString = typeString;
    }

    QVariant createValues() {
        if (type == Enum || type == AddString) {
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
    QStringList applyValue(const QVariant &val) {
        switch (type) {
        case Unknown:
            return QStringList();

        case Bool:
            if (val.type() == QVariant::Bool) {
                if (val.toBool())
                    return QStringList() << "-" + name;
                else
                    return QStringList() << "-no-" + name;
            }
            break;

        case Enum:
        case String:
        case OptionalString:
            if (val.type() == QVariant::String)
                return QStringList() << "-" + name << val.toString();
            break;

        case AddString: {
            QStringList ret;
            if (val.type() == QVariant::List) {
                auto list = val.toList();
                foreach (QVariant v, list)
                    ret << "-" + name << v.toString();
                return ret;
            }
            break;
        }
        case Void:
            if (val.type() == QVariant::Bool && val.toBool())
                return QStringList() << "-" + name;
            break;
        }

        return QStringList();
    }
};
Q_DECLARE_METATYPE(Option*);

struct Feature {
    QString name;
    QString label;
    QString purpose;
    QString section;
    QString moduleName;
    QString filePath;
    QStringList condition;
};
Q_DECLARE_METATYPE(Feature*);

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
