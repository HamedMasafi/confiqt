#include "pageselectplatform.h"

PageSelectPlatform::PageSelectPlatform(ConfigManager *config, QWidget *parent)
    : WizardPageBase(config, parent)
{
    setupUi(this);
    connect(_config, &ConfigManager::platformsUpdated,
            this, &PageSelectPlatform::config_platformsUpdated);

}

void PageSelectPlatform::config_platformsUpdated()
{
    listWidgetPlatform->clear();
    listWidgetPlatform->addItems(_config->platforms());
    labelDefaultMkspec->setText(_config->defaultPlatform());
}
