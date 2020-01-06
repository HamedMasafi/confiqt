#include "pagewelcome.h"

PageWelcome::PageWelcome(ConfigManager *config, QWidget *parent)
    : WizardPageBase (config, parent)
{
    setupUi(this);
    registerField("simpleMode", radioButtonSimpleMode);
    QPixmap logo(":/QtIcon.png");
    setPixmap(QWizard::LogoPixmap, logo);
}
