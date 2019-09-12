#ifndef QTLITEDIALOG_H
#define QTLITEDIALOG_H

#ifdef WEB_REQUEST_LIB

#include "ui_qtlitedialog.h"

class VariantRequest;
class QPushButton;
class QtLiteDialog : public QDialog, private Ui::QtLiteDialog
{
    Q_OBJECT
    VariantRequest *modulesRequest;
    VariantRequest *featuresRequest;
    QString tag;
    QStringList _features;
    QPushButton *refreashFeaturesButton;

public:
    explicit QtLiteDialog(QWidget *parent = nullptr);

    QStringList features() const;

private slots:
    void loadingTextChanged(QStringList loadingTexts);
    void on_tagsRequest_finished(QVariant data);
    void on_modulesRequest_finished(QVariant data);
    void on_featuresRequest_finished(QVariant data);
    void on_comboBoxModules_activated(const QString &arg1);
    void on_buttonBox_clicked(QAbstractButton *button);
    void on_pushButtonSelectAll_clicked();
};

#endif

#endif // QTLITEDIALOG_H
