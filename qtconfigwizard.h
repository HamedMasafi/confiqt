#ifndef QTCONFIGWIZARD_H
#define QTCONFIGWIZARD_H

#include "ui_qtconfigwizard.h"

class ConfigManager;
class QtConfigWizard : public QWizard, private Ui::QtConfigWizard
{
    Q_OBJECT
    ConfigManager *_config;

public:
    explicit QtConfigWizard(QWidget *parent = nullptr);
private slots:
    void on_QtConfigWizard_accepted();
};

#endif // QTCONFIGWIZARD_H
