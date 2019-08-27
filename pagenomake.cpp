#include "pagenomake.h"
#include <QDebug>

PageNoMake::PageNoMake(ConfigManager *config, QWidget *parent) :
    WizardPageBase(config, parent)
{
    setupUi(this);
    connect(_config, &ConfigManager::buildPathChanged, this, &PageNoMake::config_buildPathChanged);
    for (int i = 0; i < listWidgetNoMake->count(); ++i) {
        auto item = listWidgetNoMake->item(i);
        item->setFlags(item->flags() | Qt::ItemIsEditable);
        item->setCheckState(Qt::Unchecked);
    }
}

void PageNoMake::config_buildPathChanged()
{
    qDebug() << Q_FUNC_INFO;
    auto nomake = _config->nomake();
    for (int i = 0; i < listWidgetNoMake->count(); ++i) {
        QString name = listWidgetNoMake->item(i)->text();
        listWidgetNoMake->item(i)->setCheckState(nomake.contains(name)
                                                 ? Qt::Unchecked
                                                 : Qt::Checked);
    }
}
