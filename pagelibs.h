#ifndef PAGELIBS_H
#define PAGELIBS_H

#include "ui_pagelibs.h"
#include "wizardpagebase.h"

class QSortFilterProxyModel;
class QStandardItemModel;
class PageLibs : public WizardPageBase, private Ui::PageLibs
{
    Q_OBJECT

    QSortFilterProxyModel *optionsFilter;
    QStandardItemModel *optionsModel;

public:
    explicit PageLibs(ConfigManager *config, QWidget *parent = nullptr);
private slots:
    void config_configuresUpdated();

    void on_lineEditFilterOption_textChanged(const QString &arg1);

public:
    bool validatePage();
};

#endif // PAGELIBS_H
