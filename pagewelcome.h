#ifndef PAGEWELCOME_H
#define PAGEWELCOME_H

#include "ui_pagewelcome.h"
#include "wizardpagebase.h"

class PageWelcome : public WizardPageBase, private Ui::PageWelcome
{
    Q_OBJECT

public:
    explicit PageWelcome(ConfigManager *config, QWidget *parent = nullptr);
};

#endif // PAGEWELCOME_H
