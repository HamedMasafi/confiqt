#include "optionsselectdelegate.h"
#include "pagelibs.h"

#include <QSortFilterProxyModel>
#include <QStandardItemModel>

PageLibs::PageLibs(ConfigManager *config, QWidget *parent)
    : WizardPageBase(config, parent)
{
    setupUi(this);

    optionsModel = new QStandardItemModel(this);
    optionsFilter = new QSortFilterProxyModel(this);
    optionsFilter->setFilterKeyColumn(0);
    tableViewOptions->setModel(optionsFilter);
    tableViewOptions->setItemDelegateForColumn(2, new OptionsSelectDelegate(this));

    connect(_config, &ConfigManager::configuresUpdated,
            this, &PageLibs::config_configuresUpdated);
}

void PageLibs::config_configuresUpdated()
{
    QList<Option*> opts = _config->options();
    optionsModel->clear();
    optionsModel->setHorizontalHeaderLabels(QStringList() << "Name" << "Type" << "Value");
    optionsModel->setColumnCount(3);
    optionsModel->setRowCount(opts.count());
    for (int i = 0; i < opts.count(); ++i) {
        auto k = opts.at(i);
        optionsModel->setData(optionsModel->index(i, 0), k->name);
        optionsModel->setData(optionsModel->index(i, 1), k->typeString);
        auto item = optionsModel->item(i, 0);
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        item = optionsModel->item(i, 1);
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);

        optionsModel->setData(optionsModel->index(i, 2), k->createValues(), DropDownRole);
        optionsModel->setData(optionsModel->index(i, 2), static_cast<int>(k->type), TypeRole);
        optionsModel->setData(optionsModel->index(i, 2), QVariant(), Qt::DisplayRole);
    }
    optionsFilter->setSourceModel(optionsModel);
}

bool PageLibs::validatePage()
{
    _config->clearOptionsStates();
    for (int i = 0; i < optionsModel->rowCount(); ++i) {
        QString state = optionsModel->data(optionsModel->index(i, 2), Qt::DisplayRole).toString();
        QString name = optionsModel->data(optionsModel->index(i, 0)).toString();

        if (state != QString())
            _config->setOptionsState(name, state);
    }
    return true;
}

void PageLibs::on_lineEditFilterOption_textChanged(const QString &s)
{
    optionsFilter->setFilterFixedString(s);
}
