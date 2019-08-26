#ifndef PAGELICENSE_H
#define PAGELICENSE_H

#include "ui_pagelicense.h"
#include "wizardpagebase.h"

class PageLicense : public WizardPageBase, private Ui::PageLicense
{
    Q_OBJECT

public:
    explicit PageLicense(ConfigManager *config, QWidget *parent = nullptr);
private slots:
    void licensesUpdated();
    void on_comboBox_currentTextChanged(const QString &arg1);

public:
    bool validatePage();
};

#endif // PAGELICENSE_H
