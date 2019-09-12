#ifndef OPTION_H
#define OPTION_H

#include <QString>
#include <QVariant>



class Option
{
public:
    Option();
    virtual ~Option();

    enum Type {
        Unknown,
        Bool,
        Enum,
        String,
        OptionalString,
        AddString,
        Void
    };

    static Option *fromJson(const QJsonValue& json);
    static Option *fromType(const QString &type);
    virtual QVariant dropDown() const;
    virtual QStringList applyValue(const QVariant &val) const = 0;
    virtual bool readCommandLine(const QString &arg, QVariant &var, QByteArrayList &opts) = 0;

    QString name() const;
    void setName(const QString &name);

    Type type() const;
    void setType(const Type &type);

    QString typeString() const;
    void setTypeString(const QString &typeString);

    QVariant values() const;
    void setValues(const QVariant &values);

    QString moduleName() const;
    void setModuleName(const QString &moduleName);

    QString filePath() const;
    void setFilePath(const QString &filePath);

    QVariant value() const;
    void setValue(const QVariant &value);

    virtual QString displayText() const;
protected:
    Option(Type type);
    QString _name;
    Type _type;
    QString _typeString;
    QVariant _values;
    QString _moduleName;
    QString _filePath;

    QVariant _value;
    friend class JsonConfig;
};
Q_DECLARE_METATYPE(Option*);

class OptionBool : public Option
{
public:
    OptionBool();
    virtual ~OptionBool() override;

    QStringList applyValue(const QVariant &val) const override;
    bool readCommandLine(const QString &arg, QVariant &var, QByteArrayList &opts) override;
};

class OptionEnum : public Option
{
public:
    OptionEnum();
    virtual ~OptionEnum() override;

    QVariant dropDown() const override;
    QStringList applyValue(const QVariant &val) const override;
    bool readCommandLine(const QString &arg, QVariant &var, QByteArrayList &opts) override;
};

class OptionString : public Option
{
public:
    OptionString();
    virtual ~OptionString() override;

    QStringList applyValue(const QVariant &val) const override;
    bool readCommandLine(const QString &arg, QVariant &var, QByteArrayList &opts) override;
};

class OptionOptionalString : public Option
{
public:
    OptionOptionalString();
    virtual ~OptionOptionalString() override;

    QStringList applyValue(const QVariant &val) const override;
    bool readCommandLine(const QString &arg, QVariant &var, QByteArrayList &opts) override;
};

class OptionAddString : public Option
{
public:
    OptionAddString();
    virtual ~OptionAddString() override;

    QVariant dropDown() const override;
    QStringList applyValue(const QVariant &val) const override;
    bool readCommandLine(const QString &arg, QVariant &var, QByteArrayList &opts) override;
    QString displayText() const override;
};

class OptionVoid : public Option
{
public:
    OptionVoid();
    virtual ~OptionVoid() override;

    QStringList applyValue(const QVariant &val) const override;
    bool readCommandLine(const QString &arg, QVariant &var, QByteArrayList &opts) override;
};

#endif // OPTION_H
