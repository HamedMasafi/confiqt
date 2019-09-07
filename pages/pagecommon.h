#ifndef PAGECOMMON_H
#define PAGECOMMON_H

#include "ui_pagecommon.h"
#include "wizardpagebase.h"

class PageCommon : public WizardPageBase, private Ui::PageCommon
{
    Q_OBJECT

public:
    explicit PageCommon(ConfigManager *config, QWidget *parent = nullptr);
};

#endif // PAGECOMMON_H
