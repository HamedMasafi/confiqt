#ifndef PAGERUN_H
#define PAGERUN_H

#include "ui_pagerun.h"
#include "wizardpagebase.h"

class PageRun : public WizardPageBase, private Ui::PageRun
{
    Q_OBJECT

    void run();
public:
    explicit PageRun(ConfigManager *config, QWidget *parent = nullptr);

    // QWizardPage interface
public:
    void initializePage();

    // QWizardPage interface
public:
    bool isComplete() const;
};

#endif // PAGERUN_H
