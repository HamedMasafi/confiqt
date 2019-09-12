#include "optionsselectdelegate.h"
#include "pageoptions.h"
#include "option.h"
#include "optionsmodel.h"

#include <QStandardItemModel>
#include <optionsfilterproxy.h>

PageOptions::PageOptions(ConfigManager *config, QWidget *parent)
    : WizardPageBase(config, parent)
{
    setupUi(this);

    optionsModel = _config->optionsModel();
    optionsFilter = new OptionsFilterProxy(this);
    optionsFilter->setFilterKeyColumn(0);
    optionsFilter->setSourceModel(optionsModel);
    tableViewOptions->setModel(optionsFilter);
    tableViewOptions->setItemDelegateForColumn(2, new OptionsSelectDelegate(this));

    connect(_config, &ConfigManager::configuresUpdated,
            this, &PageOptions::config_configuresUpdated);
}

bool PageOptions::validatePage()
{
    return true;
}

void PageOptions::on_lineEditFilterOption_textChanged(const QString &s)
{
    optionsFilter->setSearchName(s);
}

void PageOptions::on_tableViewOptions_activated(const QModelIndex &index)
{
    auto idx = optionsFilter->index(index.row(), 0, index.parent());

    auto option = optionsModel->option(optionsFilter->mapToSource(idx));

    if (option) {
        labelName->setText(option->name());
        labelModule->setText(option->moduleName());
        labelType->setText(option->typeString());
        labelPath->setText(option->filePath());
    }
}

void PageOptions::on_comboBoxModules_activated(const QString &s)
{
    optionsFilter->setModuleName(s);
}

void PageOptions::config_configuresUpdated()
{
    comboBoxModules->clear();
    comboBoxModules->addItem("");
    comboBoxModules->addItems(_config->selectedModules());
}
