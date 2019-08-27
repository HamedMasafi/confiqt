#ifndef PAGERUN_H
#define PAGERUN_H

#include "ui_pagerun.h"
#include "wizardpagebase.h"

class PageRun : public WizardPageBase, private Ui::PageRun
{
    Q_OBJECT

public:
    explicit PageRun(ConfigManager *config, QWidget *parent = nullptr);
};

#endif // PAGERUN_H
