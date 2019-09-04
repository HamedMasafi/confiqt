#ifndef PAGEMODULES_H
#define PAGEMODULES_H

#include "ui_pagemodules.h"
#include "wizardpagebase.h"

class PageModules : public WizardPageBase, private Ui::PageModules
{
    Q_OBJECT

public:
    explicit PageModules(ConfigManager *config, QWidget *parent = nullptr);

private slots:
    void config_modulesUpdated();

    // QWizardPage interface
public:
    bool validatePage();
};

#endif // PAGEMODULES_H
