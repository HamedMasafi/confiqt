#include "optionsmodel.h"

#include "option.h"
#include <configmanager.h>

OptionsModel::OptionsModel(ConfigManager *parent) : QAbstractTableModel(parent)
  ,_config(parent), d(parent->d)
{
    connect(_config, &ConfigManager::configuresUpdated, this, &OptionsModel::config_configuresUpdated);
}

void OptionsModel::setState(const QString &name, const QVariant &value)
{
    auto it = d->options.begin();
    auto i = 0;
    while (it != d->options.end()) {
        if ((*it)->name() == name) {

            if ((*it)->type() == Option::AddString) {
                auto list = d->optionsStates.value((*it)->name()).toList();
                list.append(value);
                d->optionsStates.insert((*it)->name(), list);
            } else {
                d->optionsStates.insert((*it)->name(), value);
            }

            auto idx = index(i, 2);
            setData(idx, value);
            emit dataChanged(idx, idx);
            break;
        }
        ++i;
        ++it;
    }
}

Option *OptionsModel::option(const QModelIndex &index) const
{
    if (index.row() < 0)
        return nullptr;
    if (index.row() >= d->options.count())
        return nullptr;
    return d->options.at(index.row());
}

int OptionsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return d->options.count();
}

int OptionsModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 3;
}

QVariant OptionsModel::data(const QModelIndex &index, int role) const
{
    auto opt = d->options.at(index.row());
    if (role == Qt::DisplayRole && opt){
        switch (index.column()) {
        case 0:
            return opt->name();
        case 1:
            return opt->typeString();
        case 2:
            return d->optionsStates.value(opt->name());
        default:
            Q_UNREACHABLE();
        }
    }

    if (opt && index.column() == 2) {
        if (role == TypeRole){
            return opt->type();
        }
        if (role == DropDownRole){
            return opt->dropDown();
        }
        if (role == Qt::EditRole) {
            return d->optionsStates.value(opt->name());
        }
        if (role == DataRole) {
            return QVariant::fromValue(opt);
        }
    }
    return QVariant();
}


void OptionsModel::config_configuresUpdated()
{
    beginRemoveRows(QModelIndex(), 0, 0);
    endRemoveRows();

    beginInsertRows(QModelIndex(), 0, _config->options().count() - 1);
    endInsertRows();
}

QVariant OptionsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Vertical)
            return section + 1;
        switch (section) {
        case 0:
            return "Name";
        case 1:
            return "Type";
        case 2:
            return "Value";
        default:
            Q_UNREACHABLE();
        }
    }
    return QVariant();
}

Qt::ItemFlags OptionsModel::flags(const QModelIndex &index) const
{
    auto f = QAbstractTableModel::flags(index);
    if (index.column() == 2)
        f |= Qt::ItemIsEditable;
    return f;
}

bool OptionsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.column() == 2 && role == Qt::DisplayRole) {
        auto o = option(index);
        if (!o)
            return false;
        d->optionsStates.insert(o->name(), value);
        return true;
    }
    return QAbstractTableModel::setData(index, value, role);
}
