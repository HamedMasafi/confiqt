#include "pagecommon.h"

PageCommon::PageCommon(ConfigManager *config, QWidget *parent) :
    WizardPageBase(config, parent)
{
    setupUi(this);
}
