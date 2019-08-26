#ifndef PAGESELECTPATHS_H
#define PAGESELECTPATHS_H

#include "ui_pageselectpaths.h"
#include "wizardpagebase.h"

class PageSelectPaths : public WizardPageBase, private Ui::PageSelectPaths
{
    Q_OBJECT

public:
    explicit PageSelectPaths(ConfigManager *config, QWidget *parent = nullptr);

    // QWizardPage interface
public:
    bool validatePage();
};

#endif // PAGESELECTPATHS_H
