#ifndef PAGERUN_H
#define PAGERUN_H

#include "ui_pagerun.h"
#include "wizardpagebase.h"

class PageRun : public WizardPageBase, private Ui::PageRun
{
    Q_OBJECT

public:
    explicit PageRun(ConfigManager *config, QWidget *parent = nullptr);

    // QWizardPage interface
public:
    void initializePage();

    // QWizardPage interface
public:
    bool isComplete() const;
private slots:
    void initProcess(QProcess *p);
    void configure();
    void make();
    void detectFeatures();
};

#endif // PAGERUN_H
