#ifndef PAGECROSSCOMPILE_H
#define PAGECROSSCOMPILE_H

#include "ui_pagecrosscompile.h"
#include "wizardpagebase.h"

class PageCrossCompile : public WizardPageBase, private Ui::PageCrossCompile
{
    Q_OBJECT

public:
    explicit PageCrossCompile(ConfigManager *config, QWidget *parent = nullptr);

private slots:
    void config_configuresUpdated();
    void on_comboBoxType_currentIndexChanged(int index);

public:
    bool validatePage();
};

#endif // PAGECROSSCOMPILE_H
