#include "pagewelcome.h"

#include <editors/multistringselector.h>

PageWelcome::PageWelcome(ConfigManager *config, QWidget *parent)
    : WizardPageBase (config, parent)
{
    setupUi(this);
    registerField("simpleMode", radioButtonSimpleMode);
    auto w = new MultiStringSelector;
    w->setValue(QVariantList() << "One" << "Two" << "Three");
    layout()->addWidget(w);
}
