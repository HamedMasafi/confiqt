#ifndef PAGEOPTIONS_H
#define PAGEOPTIONS_H

#include "ui_pageoptions.h"
#include "wizardpagebase.h"

class QSortFilterProxyModel;
class QStandardItemModel;
class PageOptions : public WizardPageBase, private Ui::PageOptions
{
    Q_OBJECT

    QSortFilterProxyModel *optionsFilter;
    QStandardItemModel *optionsModel;

public:
    explicit PageOptions(ConfigManager *config, QWidget *parent = nullptr);
private slots:
    void config_configuresUpdated();

    void on_lineEditFilterOption_textChanged(const QString &arg1);

    void on_tableViewOptions_activated(const QModelIndex &index);

public:
    bool validatePage();
};

#endif // PAGEOPTIONS_H
