#include "pagecrosscompile.h"
#include <QDebug>

PageCrossCompile::PageCrossCompile(ConfigManager *config, QWidget *parent)
    : WizardPageBase(config, parent)
{
    setupUi(this);
    connect(_config, &ConfigManager::configuresUpdated,
            this, &PageCrossCompile::config_configuresUpdated);

}

void PageCrossCompile::config_configuresUpdated()
{
    QVariant platform = _config->optionState("platform");
    QVariant xplatform = _config->optionState("xplatform");

    if (platform == QVariant() && xplatform == QVariant()) {
        checkBoxSelectPLatform->setChecked(false);
        comboBoxType->setCurrentText(0);
        return;
    }

    checkBoxSelectPLatform->setChecked(true);
    comboBoxType->setCurrentIndex(platform == QVariant() ? 1 : 0);
    auto items = listWidgetPlatform->findItems(platform == QVariant() ? xplatform.toString()
                                                                      : platform.toString(),
                                               Qt::MatchExactly);

    if (!items.count())
        return;
    listWidgetPlatform->setCurrentItem(items.first());
}

bool PageCrossCompile::validatePage()
{
    if (!checkBoxSelectPLatform->isChecked())
        return true;

    if (listWidgetPlatform->currentRow() == -1)
        return false;

    if (comboBoxType->currentIndex() == 0) {
        _config->setOptionsState("platform", listWidgetPlatform->currentItem()->text());
        _config->setOptionsState("xplatform", QVariant());
    } else {
        _config->setOptionsState("xplatform", listWidgetPlatform->currentItem()->text());
        _config->setOptionsState("platform", QVariant());
    }

    return true;
}


void PageCrossCompile::on_comboBoxType_currentIndexChanged(int index)
{
    listWidgetPlatform->clear();

    switch (index) {
    case 0:
        listWidgetPlatform->addItems(_config->platforms());
        break;
    case 1:
        listWidgetPlatform->addItems(_config->devices());
        break;
    }
}
