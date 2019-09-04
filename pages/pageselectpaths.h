#ifndef PAGESELECTPATHS_H
#define PAGESELECTPATHS_H

#include "ui_pageselectpaths.h"
#include "wizardpagebase.h"

class PageSelectPaths : public WizardPageBase, private Ui::PageSelectPaths
{
    Q_OBJECT

public:
    explicit PageSelectPaths(ConfigManager *config, QWidget *parent = nullptr);

    // QWizardPage interface
public:
    bool validatePage();
private slots:
    void on_pushButtonSelectSourcePath_clicked();
    void on_pushButtonSelectBuildPath_clicked();
    void on_pushButtonSelectInstallPath_clicked();
};

#endif // PAGESELECTPATHS_H
