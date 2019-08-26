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
    listWidgetPlatform->clear();
    listWidgetPlatform->addItems(_config->platforms());
}

