#include "pagemodules.h"

PageModules::PageModules(ConfigManager *config, QWidget *parent)
    : WizardPageBase (config, parent)
{
    setupUi(this);
    connect(_config, &ConfigManager::modulesUpdated, this, &PageModules::config_modulesUpdated);
}

void PageModules::config_modulesUpdated()
{
    QStringList modules = _config->submodules();
    listWidgetModules->clear();
    foreach (QString module, modules) {
        QListWidgetItem *item = new QListWidgetItem(listWidgetModules);
        item->setText(module);
        item->setFlags(item->flags() | Qt::ItemIsEditable);
        item->setCheckState(Qt::Checked);
        listWidgetModules->addItem(item);
    }
}

bool PageModules::validatePage()
{
    QStringList modules;
    for (int i = 0; i < listWidgetModules->count(); ++i) {
        auto item = listWidgetModules->item(i);
        if (item->checkState() == Qt::Checked)
            modules.append(item->text());
    }
    if (!modules.count())
        return false;
    _config->setSelectedModules(modules);
    return true;
}
