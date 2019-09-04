#include "pagenomake.h"
#include <QDebug>

PageNoMake::PageNoMake(ConfigManager *config, QWidget *parent) :
    WizardPageBase(config, parent)
{
    setupUi(this);

    connect(_config, &ConfigManager::configuresUpdated,
            this, &PageNoMake::config_configuresUpdated);
}

void PageNoMake::config_configuresUpdated()
{
    listWidgetNoMake->clear();
    Option *nomake = _config->option("nomake");
    if (nomake == nullptr)
        return;
    if (nomake->values.type() == QVariant::List) {
        auto list = nomake->values.toList();
        foreach (QVariant v, list) {
            QListWidgetItem *item = new QListWidgetItem(v.toString(), listWidgetNoMake);
            item->setFlags(item->flags() | Qt::ItemIsEditable);
            item->setCheckState(Qt::Unchecked);
            listWidgetNoMake->addItem(item);
        }
     }
}
