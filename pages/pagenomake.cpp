#include "pagenomake.h"
#include "option.h"

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
    if (nomake->values().type() == QVariant::List) {
        auto list = nomake->values().toList();
        foreach (QVariant v, list) {
            QListWidgetItem *item = new QListWidgetItem(v.toString(), listWidgetNoMake);
            item->setFlags(item->flags() | Qt::ItemIsEditable);
            item->setCheckState(list.contains(v) ? Qt::Checked : Qt::Unchecked);
            listWidgetNoMake->addItem(item);
        }
    }
}

bool PageNoMake::validatePage()
{
    QVariantList nomake;
    for (int i = 0; i < listWidgetNoMake->count(); ++i) {
        if (listWidgetNoMake->item(i)->checkState() == Qt::Checked)
            nomake << listWidgetNoMake->item(i)->text();
    }
    _config->setOptionsState("nomake", nomake);
    return true;
}
