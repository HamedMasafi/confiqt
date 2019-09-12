#ifdef WEB_REQUEST_LIB

#include "qtlitedialog.h"
#include <variantrequest.h>
#include <QDebug>
#include <QPushButton>
#include <webrequestmanager.h>

QStringList QtLiteDialog::features() const
{
    return _features;
}

void QtLiteDialog::loadingTextChanged(QStringList loadingTexts)
{
    labelStatus->setText(loadingTexts.join(", "));
}

QtLiteDialog::QtLiteDialog(QWidget *parent) :
    QDialog(parent)
{
    VariantRequest *tagsRequest = new VariantRequest(this);
    tagsRequest->setObjectName("tagsRequest");
    tagsRequest->setUrl(QUrl("https://qtlite.com/tags.json"));
    tagsRequest->setMethod(WebRequest::Get);
    tagsRequest->setExpirationSeconds(60 * 60 * 24);
    tagsRequest->setLoadingText("Getting tags");
    tagsRequest->setIncludeDataInCacheId(true);

    modulesRequest = new VariantRequest(this);
    modulesRequest->setObjectName("modulesRequest");
    modulesRequest->setMethod(WebRequest::Get);
    modulesRequest->setExpirationSeconds(60 * 60 * 24);
    modulesRequest->setLoadingText("Getting modules");
    modulesRequest->setIncludeDataInCacheId(true);

    featuresRequest = new VariantRequest(this);
    featuresRequest->setObjectName("featuresRequest");
    featuresRequest->setMethod(WebRequest::Get);
    featuresRequest->setExpirationSeconds(60 * 60 * 24);
    featuresRequest->setLoadingText("Getting features");
    featuresRequest->setIncludeDataInCacheId(true);

    setupUi(this);
    connect(WebRequestManager::instance(), &WebRequestManager::loadingTextsChanged,
            this, &QtLiteDialog::loadingTextChanged);
    buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    refreashFeaturesButton = new QPushButton(this);
    refreashFeaturesButton->setText("Refresh features");
    buttonBox->addButton(refreashFeaturesButton, QDialogButtonBox::HelpRole);
    tagsRequest->send();
}

void QtLiteDialog::on_tagsRequest_finished(QVariant data)
{
    if (data.type() == QVariant::Map) {
        comboBoxModules->clear();
        auto list = data.toMap().value("tags").toList();
        foreach (QVariant v, list)
            comboBoxModules->addItem(v.toMap().value("name").toString());

        if (comboBoxModules->count())
            on_comboBoxModules_activated(comboBoxModules->itemText(0));
    }
}

void QtLiteDialog::on_modulesRequest_finished(QVariant data)
{
    if (data.type() == QVariant::Map) {
        listWidgetModules->clear();
        auto list = data.toMap().value("modules").toList();
        foreach (QVariant v, list) {
            QListWidgetItem *item = new QListWidgetItem(listWidgetModules);
            item->setCheckState(Qt::Unchecked);
            item->setText(v.toString());
            listWidgetModules->addItem(item);
        }
    }
}

void QtLiteDialog::on_featuresRequest_finished(QVariant data)
{
    if (data.type() == QVariant::Map) {
        _features.clear();
        listWidgetFeatures->clear();
        auto list = data.toMap().value("features").toList();
        foreach (QVariant v, list) {
            _features.append(v.toString());
            listWidgetFeatures->addItem(v.toString());
        }
        buttonBox->button(QDialogButtonBox::Ok)->setEnabled(list.count());
    }
}

void QtLiteDialog::on_comboBoxModules_activated(const QString &s)
{
    listWidgetModules->clear();
    listWidgetFeatures->clear();
    tag = s;
    modulesRequest->setUrl(QUrl("https://qtlite.com/modules.json?tag=" + s));
    modulesRequest->send();
}

void QtLiteDialog::on_buttonBox_clicked(QAbstractButton *button)
{
    if (button == refreashFeaturesButton) {
        listWidgetFeatures->clear();
        QString url = "https://qtlite.com/features.json?tag=" + tag;
        for (int i = 0; i < listWidgetModules->count(); ++i) {
            auto item = listWidgetModules->item(i);
            if (item->checkState() == Qt::Checked)
                url.append("&modules%5B%5D=" + item->text());
        }
        featuresRequest->setUrl(QUrl(url));
        featuresRequest->send();
    }
}

void QtLiteDialog::on_pushButtonSelectAll_clicked()
{
    for (int i = 0; i < listWidgetModules->count(); ++i)
        listWidgetModules->item(i)->setCheckState(Qt::Checked);
}

#endif
