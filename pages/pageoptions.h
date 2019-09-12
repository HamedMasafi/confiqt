#ifndef PAGEOPTIONS_H
#define PAGEOPTIONS_H

#include "ui_pageoptions.h"
#include "wizardpagebase.h"

class OptionsFilterProxy;
class OptionsModel;
class PageOptions : public WizardPageBase, private Ui::PageOptions
{
    Q_OBJECT

    OptionsFilterProxy *optionsFilter;
    OptionsModel *optionsModel;

public:
    explicit PageOptions(ConfigManager *config, QWidget *parent = nullptr);

private slots:
    void on_lineEditFilterOption_textChanged(const QString &arg1);
    void on_tableViewOptions_activated(const QModelIndex &index);

    void on_comboBoxModules_activated(const QString &arg1);

    void config_configuresUpdated();
public:
    bool validatePage();
};

#endif // PAGEOPTIONS_H
