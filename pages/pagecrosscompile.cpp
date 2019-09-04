#include "pagecrosscompile.h"

PageCrossCompile::PageCrossCompile(ConfigManager *config, QWidget *parent)
    : WizardPageBase(config, parent)
{
    setupUi(this);
    connect(_config, &ConfigManager::platformsUpdated,
            this, &PageCrossCompile::config_platformsUpdated);

}

void PageCrossCompile::config_platformsUpdated()
{
    on_comboBoxType_activated(0);
}


void PageCrossCompile::on_comboBoxType_activated(int index)
{
    listWidgetPlatform->clear();

    switch (index) {
    case 0:
        listWidgetPlatform->addItems(_config->platforms());
        break;
    case 1:
        listWidgetPlatform->addItems(_config->devices());
        break;
    }
}

