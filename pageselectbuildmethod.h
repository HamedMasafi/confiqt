#ifndef PAGESELECTBUILDMETHOD_H
#define PAGESELECTBUILDMETHOD_H

#include "ui_pageselectbuildmethod.h"
#include "wizardpagebase.h"

class PageSelectBuildMethod : public WizardPageBase, private Ui::PageSelectBuildMethod
{
    Q_OBJECT

public:
    explicit PageSelectBuildMethod(ConfigManager *config, QWidget *parent = nullptr);
private slots:
    void on_pushButtonShowParameteres_clicked();
};

#endif // PAGESELECTBUILDMETHOD_H
