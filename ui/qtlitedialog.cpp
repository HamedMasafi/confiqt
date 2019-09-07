#include "qtlitedialog.h"
#include <jsonrequest.h>
#include <QDebug>
#include <QPushButton>

QStringList QtLiteDialog::features() const
{
    return _features;
}

QtLiteDialog::QtLiteDialog(QWidget *parent) :
    QDialog(parent)
{
    JsonRequest *tagsRequest = new JsonRequest(this);
    tagsRequest->setObjectName("tagsRequest");
    tagsRequest->setUrl(QUrl("https://qtlite.com/tags.json"));

    tagsRequest->setMethod(WebRequest::Get);
    tagsRequest->send();

    modulesRequest = new JsonRequest(this);
    modulesRequest->setObjectName("modulesRequest");
    modulesRequest->setMethod(WebRequest::Get);

    featuresRequest = new JsonRequest(this);
    featuresRequest->setObjectName("featuresRequest");
    featuresRequest->setMethod(WebRequest::Get);

    setupUi(this);
    buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    refreashFeaturesButton = new QPushButton(this);
    refreashFeaturesButton->setText("Refresh features");
    buttonBox->addButton(refreashFeaturesButton, QDialogButtonBox::HelpRole);
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
    tag = s;
    modulesRequest->setUrl(QUrl("https://qtlite.com/modules.json?tag=" + s));
    modulesRequest->send();
}

void QtLiteDialog::on_buttonBox_clicked(QAbstractButton *button)
{
    if (button == refreashFeaturesButton) {
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
