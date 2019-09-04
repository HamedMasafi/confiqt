#ifndef PAGEFINISH_H
#define PAGEFINISH_H

#include "ui_pagefinish.h"
#include "wizardpagebase.h"

class PageFinish : public WizardPageBase, private Ui::PageFinish
{
    Q_OBJECT

public:
    explicit PageFinish(ConfigManager *config, QWidget *parent = nullptr);
};

#endif // PAGEFINISH_H
