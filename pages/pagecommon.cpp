#include "pagecommon.h"

PageCommon::PageCommon(ConfigManager *config, QWidget *parent) :
    WizardPageBase(config, parent)
{
    setupUi(this);

    connect(_config, &ConfigManager::configuresUpdated,
            this, &PageCommon::config_configuresUpdated);
}

void PageCommon::config_configuresUpdated()
{
    checkBoxStatic->setChecked(_config->optionState("static").toString() == "yes");
    checkBoxDeveloperBuild->setChecked(_config->optionState("developer-build").toBool());
    checkBoxOptimizeForSize->setChecked(_config->optionState("optimize-size").toBool());
}

bool PageCommon::validatePage()
{
    if (checkBoxDeveloperBuild->isChecked())
        _config->setOptionsState("developer-build", true);

    if (checkBoxStatic->isChecked())
        _config->setOptionsState("static", "yes");

    if (checkBoxOptimizeForSize->isChecked())
        _config->setOptionsState("optimize-size", true);

    return true;
}
