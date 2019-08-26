#ifndef PAGEWELCOME_H
#define PAGEWELCOME_H

#include "ui_pagewelcome.h"

class PageWelcome : public QWizardPage, private Ui::PageWelcome
{
    Q_OBJECT

public:
    explicit PageWelcome(QWidget *parent = nullptr);
};

#endif // PAGEWELCOME_H
