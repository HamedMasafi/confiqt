#include "option.h"

#include <QJsonObject>
#include <QJsonValue>

Option::Option()
{

}

Option::~Option()
{

}

Option *Option::fromJson(const QJsonValue &json)
{
    Option *o = nullptr;
    QString type;
    if (json.isString()) {
        type = json.toString();
    } else if (json.isObject()) {
        QJsonObject obj = json.toObject();
        type = obj.value("type").toString();
    }
    o = fromType(type);
    if (!o)
        return nullptr;

    return o;
}

Option::Option(Option::Type type) : _type(type)
{}

QVariant Option::value() const
{
    return _value;
}

void Option::setValue(const QVariant &value)
{
    _value = value;
}

QString Option::displayText() const
{
    return _value.toString();
}

QString Option::filePath() const
{
    return _filePath;
}

void Option::setFilePath(const QString &filePath)
{
    _filePath = filePath;
}

QString Option::moduleName() const
{
    return _moduleName;
}

void Option::setModuleName(const QString &moduleName)
{
    _moduleName = moduleName;
}

QVariant Option::values() const
{
    return _values;
}

void Option::setValues(const QVariant &values)
{
    _values = values;
}

QString Option::typeString() const
{
    return _typeString;
}

void Option::setTypeString(const QString &typeString)
{
    _typeString = typeString;
}

Option::Type Option::type() const
{
    return _type;
}

void Option::setType(const Type &type)
{
    _type = type;
}

QString Option::name() const
{
    return _name;
}

void Option::setName(const QString &name)
{
    _name = name;
}

Option *Option::fromType(const QString &type)
{
    Option *o = nullptr;
    if (type == "boolean")
        o = new OptionBool;
    else if (type == "enum")
        o = new OptionEnum;
    else if (type == "string")
        o = new OptionString;
    else if (type == "optionalString")
        o = new OptionOptionalString;
    else if (type == "addString")
        o = new OptionAddString;
    else if (type == "void")
        o = new OptionVoid;
    else
        o = new OptionString;
    if (o)
        o->_typeString = type;

    return o;
}

QStringList Option::dropDown() const
{
    return QStringList();
}

OptionBool::OptionBool() : Option(Option::Bool)
{}

OptionBool::~OptionBool()
{

}

QStringList OptionBool::applyValue(const QVariant &val) const
{
    if (val.type() == QVariant::Int) {
        auto ch = val.value<Qt::CheckState>();
        if (ch == Qt::Checked)
            return QStringList() << "-" + _name;
        else if (ch == Qt::Unchecked)
            return QStringList() << "-no-" + _name;
    }
    return QStringList();
}

bool OptionBool::readCommandLine(const QString &arg, QVariant &var, QStringList &opts)
{
    Q_UNUSED(opts)
    if (arg == "no") {
        var = false;
        return true;
    } else if (arg == "") {
        var = true;
        return true;
    }
    return false;
}

OptionEnum::OptionEnum() : Option(Option::Enum)
{}

OptionEnum::~OptionEnum()
{

}

QStringList OptionEnum::dropDown() const
{
    QStringList list;
    if (_values.type() == QVariant::Map) {
        auto keys = _values.toMap().keys();
        foreach(QString k, keys)
            list.append(k);
    }
    if (_values.type() == QVariant::List) {
        foreach (QVariant v, _values.toList())
            list.append(v.toString());
    }
    return list;
}

QStringList OptionEnum::applyValue(const QVariant &val) const
{
    if (val.toString() == "yes")
        return QStringList() << "-" + _name;
    else
        return QStringList() << "-" + val.toString() + "-" + _name;
}

bool OptionEnum::readCommandLine(const QString &arg, QVariant &var, QStringList &opts)
{
    Q_UNUSED(opts)
    if (arg == "")
        var = "yes";
    else
        var = arg;
    return true;
}

OptionString::OptionString() : Option(Option::String)
{}

OptionString::~OptionString()
{

}

QStringList OptionString::applyValue(const QVariant &val) const
{
    return QStringList() << "-" + _name << val.toString();
}

bool OptionString::readCommandLine(const QString &arg, QVariant &var, QStringList &opts)
{
    Q_UNUSED(arg)
    var = opts.takeFirst();
    return true;
}

OptionOptionalString::OptionOptionalString() : Option(Option::OptionalString)
{}

OptionOptionalString::~OptionOptionalString()
{

}

QStringList OptionOptionalString::applyValue(const QVariant &val) const
{
    QStringList ret;
    ret << "-" + name();
    if (!val.toString().isEmpty())
        ret << val.toString();
    return ret;
}

bool OptionOptionalString::readCommandLine(const QString &arg, QVariant &var, QStringList &opts)
{
    Q_UNUSED(arg)
    var = opts.takeFirst();
    return true;
}

OptionAddString::OptionAddString() : Option(Option::AddString)
{}

OptionAddString::~OptionAddString()
{

}

QStringList OptionAddString::dropDown() const
{
    QStringList list;
    if (_values.type() == QVariant::Map) {
        auto keys = _values.toMap().keys();
        foreach(QString k, keys)
            list.append(k);
    }
    if (_values.type() == QVariant::List) {
        foreach(QVariant v,_values.toList())
            list.append(v.toString());
    }
    return list;
}

QStringList OptionAddString::applyValue(const QVariant &val) const
{
    QStringList ret;
    if (val.type() == QVariant::List) {
        auto list = val.toList();
        foreach (QVariant v, list)
            ret << "-" + _name << v.toString();
    }
    return ret;
}

bool OptionAddString::readCommandLine(const QString &arg, QVariant &var, QStringList &opts)
{
    Q_UNUSED(arg)
    auto tmp = opts.takeFirst();
    if (tmp.startsWith("-")) {
        opts.prepend(tmp);
        tmp.clear();
    }
    var = tmp;
    return true;
}

QString OptionAddString::displayText() const
{
    QString s;
    auto list = _value.toList();
    foreach (QVariant v, list) {
        if (s.isEmpty())
            s.append(", ");
        s.append(v.toString());
    }
    return s;
}


OptionVoid::OptionVoid() : Option(Option::Void)
{}

OptionVoid::~OptionVoid()
{

}

QStringList OptionVoid::applyValue(const QVariant &val) const
{
    //TODO: avoid this
    if (val.type() == QVariant::String && val.toString() == "true")
        return QStringList() << "-" + _name;
    if (val.type() == QVariant::Bool && val.toBool())
        return QStringList() << "-" + _name;
    return QStringList();
}

bool OptionVoid::readCommandLine(const QString &arg, QVariant &var, QStringList &opts)
{
    Q_UNUSED(arg)
    Q_UNUSED(opts)
    var = true;
    return true;
}

