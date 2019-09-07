#include "pagecommon.h"

PageCommon::PageCommon(ConfigManager *config, QWidget *parent) :
    WizardPageBase(config, parent)
{
    setupUi(this);
}

bool PageCommon::validatePage()
{
    if (checkBoxDeveloperBuild->isChecked())
        _config->setOptionsState("developer-build", true);

    if (checkBoxStatic->isChecked())
        _config->setOptionsState("static", true);

    if (checkBoxOptimizeForSize->isChecked())
        _config->setOptionsState("optimize-size", true);

    return true;
}
