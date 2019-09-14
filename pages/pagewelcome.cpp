#include "pagewelcome.h"

#include <editors/multistringeditor.h>
#include <editors/optionbooleditor.h>
#include <editors/multistringselector.h>
#include <editors/stringeditor.h>
#include <editors/pathselecteditor.h>
#include <editors/comboselector.h>
#include <editors/voideditor.h>
#include <editors/optionalstringeditor.h>

PageWelcome::PageWelcome(ConfigManager *config, QWidget *parent)
    : WizardPageBase (config, parent)
{
    setupUi(this);
    registerField("simpleMode", radioButtonSimpleMode);

#ifdef QT_DEBUG
    {
        auto w = new MultiStringEditor;
        w->setValue(QVariantList() << "One" << "Two" << "Three");
        layout()->addWidget(w);
    }
    {
        auto w = new MultiStringSelector;
        w->setDropDown(QStringList() << "One" << "Two" << "Three");
        w->setValue(QVariantList() << "One" << "Three");
        layout()->addWidget(w);
    }
    {
        auto w = new OptionBoolEditor;
        w->setValue(true);
        layout()->addWidget(w);
        w->show();
    }
    {
        auto w = new StringEditor;
        layout()->addWidget(w);
    }
    {
        auto w = new PathSelectEditor;
        layout()->addWidget(w);
        w->show();
    }
    {
        auto w = new ComboSelector;
        w->setDropDown(QStringList() << "One" << "Two" << "Three");
        w->setValue("Two");
        layout()->addWidget(w);
    }
    {
        auto w = new VoidEditor;
        w->setValue(true);
        layout()->addWidget(w);
    }
    {
        auto w = new OptionalStringEditor;
        w->setValue(true);
        layout()->addWidget(w);
    }
#endif
}
