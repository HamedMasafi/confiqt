#include "configmanager.h"
#include "saveconfirmdialog.h"

#include <QFileDialog>

SaveConfirmDialog::SaveConfirmDialog(ConfigManager *config, QWidget *parent) :
    QDialog(parent), _config(config)
{
    setupUi(this);
}

void SaveConfirmDialog::on_pushButtonSelectPath_clicked()
{
    QString path = QFileDialog::getSaveFileName(this, tr("Select file"), _config->buildPath());
    if (path != QString())
        lineEditPath->setText(path);
}

void SaveConfirmDialog::on_buttonBox_accepted()
{
    QString path;
    ConfigManager::SaveParametereType params;

    if (radioButtonSaveCustomLocation->isChecked())
        path = lineEditPath->text();

    if (checkBoxOptions->isChecked()) params |= ConfigManager::Options;
    if (checkBoxFeatures->isChecked()) params |= ConfigManager::Features;
    if (checkBoxCommon->isChecked()) params |= ConfigManager::Commons;

    if (radioButtonEverything->isChecked())
        params = ConfigManager::All;

    bool ok;

    if (radioButtonSaveCustomLocation)
        ok = _config->exportJson(path, params);
    else
        ok = _config->save(path, params);

    if (ok)
        accept();
}

void SaveConfirmDialog::on_buttonBox_clicked(QAbstractButton *button)
{
    if (buttonBox->buttonRole(button) == QDialogButtonBox::DestructiveRole)
        accept();
}
