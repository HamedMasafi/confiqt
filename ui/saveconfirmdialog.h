#ifndef SAVECONFIRMDIALOG_H
#define SAVECONFIRMDIALOG_H

#include "ui_saveconfirmdialog.h"

class ConfigManager;
class SaveConfirmDialog : public QDialog, private Ui::SaveConfirmDialog
{
    Q_OBJECT
    ConfigManager *_config;

public:
    explicit SaveConfirmDialog(ConfigManager *config, QWidget *parent = nullptr);
private slots:
    void on_pushButtonSelectPath_clicked();
    void on_buttonBox_accepted();
};

#endif // SAVECONFIRMDIALOG_H
