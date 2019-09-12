#ifndef OPTIONSMODEL_H
#define OPTIONSMODEL_H

#include <QAbstractTableModel>
#include <QExplicitlySharedDataPointer>
#include <configdata.h>

class ConfigManager;
class ConfigData;
class OptionsModel : public QAbstractTableModel
{
    Q_OBJECT
    ConfigManager *_config;
    QExplicitlySharedDataPointer<ConfigData> d;

public:
    OptionsModel(ConfigManager *parent);

    void setState(const QString &name, const QVariant& value);

    Option *option(const QModelIndex &index) const;

private slots:
    void config_configuresUpdated();

public:
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
};

#endif // OPTIONSMODEL_H
