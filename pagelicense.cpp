#include "pagelicense.h"

#include <QMessageBox>

PageLicense::PageLicense(ConfigManager *config, QWidget *parent) :
    WizardPageBase(config, parent)
{
    setupUi(this);
    connect(_config, &ConfigManager::licensesUpdated, this, &PageLicense::licensesUpdated);
}

void PageLicense::licensesUpdated() {
    comboBox->clear();
    comboBox->addItems(_config->licenses());
}

bool PageLicense::validatePage()
{
    if (!checkBoxConfirmLicense->isChecked()) {
        QMessageBox::warning(this, title(),
                             "You must confirm license agreement to avoid script input interaction");
        return false;
    }
    _config->setConfirmLicense(checkBoxConfirmLicense->isChecked());
    _config->setUseCommercial(comboBoxLicenseType->currentIndex() == 1);

    return true;
}

void PageLicense::on_comboBox_currentTextChanged(const QString &s)
{
    QString path = QString("%1/LICENSE.%2").arg(_config->sourcePath(), s);
    QFile lf(path);
    if (!lf.open(QIODevice::Text | QIODevice::ReadOnly))
        return;

    plainTextEdit->setPlainText(lf.readAll());
    lf.close();
}
