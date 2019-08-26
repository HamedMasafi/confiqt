#include "pageselectbuildmethod.h"

PageSelectBuildMethod::PageSelectBuildMethod(ConfigManager *config, QWidget *parent)
    : WizardPageBase(config, parent)
{
    setupUi(this);
    plainTextEditParameteres->hide();
}

void PageSelectBuildMethod::on_pushButtonShowParameteres_clicked()
{
    plainTextEditParameteres->show();
    plainTextEditParameteres->setPlainText(_config->createCommand().join(" "));
}
