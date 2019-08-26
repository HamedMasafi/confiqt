#include "qtconfigwizard.h"
#include "wizardpagebase.h"


WizardPageBase::WizardPageBase(ConfigManager *config, QWidget *parent)
    : QWizardPage (parent), _config(config)
{

}

ConfigManager *WizardPageBase::config() const
{
    return _config;
}

void WizardPageBase::setConfig(ConfigManager *config)
{
    _config = config;
}

QtConfigWizard *WizardPageBase::wizard() const {
    return qobject_cast<QtConfigWizard*>(QWizardPage::wizard());
}
