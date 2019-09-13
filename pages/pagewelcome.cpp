#include "pagewelcome.h"

#include <editors/multistringeditor.h>
#include <editors/optionbooleditor.h>
#include <editors/multistringselector.h>
#include <editors/stringeditor.h>

PageWelcome::PageWelcome(ConfigManager *config, QWidget *parent)
    : WizardPageBase (config, parent)
{
    setupUi(this);
    registerField("simpleMode", radioButtonSimpleMode);
    {
        auto w = new MultiStringEditor;
        w->setValue(QVariantList() << "One" << "Two" << "Three");
        layout()->addWidget(w);
    }
    {
        auto w = new MultiStringSelector;
        w->setValue(QVariantList() << "One" << "Two" << "Three");
        layout()->addWidget(w);
    }
    {
        auto w = new OptionBoolEditor;
        w->setValue(true);
        layout()->addWidget(w);
    }
    {
        auto w = new StringEditor;
        layout()->addWidget(w);
    }
}
