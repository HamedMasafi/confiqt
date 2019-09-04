#ifndef PAGESELECTPLATFORM_H
#define PAGESELECTPLATFORM_H

#include "ui_pageselectplatform.h"
#include "wizardpagebase.h"

class PageSelectPlatform : public WizardPageBase, private Ui::PageSelectPlatform
{
    Q_OBJECT

public:
    explicit PageSelectPlatform(ConfigManager *config, QWidget *parent = nullptr);

public slots:
    void config_platformsUpdated();
};

#endif // PAGESELECTPLATFORM_H
