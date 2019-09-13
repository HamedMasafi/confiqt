#ifndef PAGECOMMON_H
#define PAGECOMMON_H

#include "ui_pagecommon.h"
#include "wizardpagebase.h"

class PageCommon : public WizardPageBase, private Ui::PageCommon
{
    Q_OBJECT

    QMap<QCheckBox*, QString> map;

public:
    explicit PageCommon(ConfigManager *config, QWidget *parent = nullptr);

public:
    bool validatePage();
private slots:
    void config_configuresUpdated();
};

#endif // PAGECOMMON_H
