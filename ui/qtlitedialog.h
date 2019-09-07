#ifndef QTLITEDIALOG_H
#define QTLITEDIALOG_H

#include "ui_qtlitedialog.h"

class JsonRequest;
class QPushButton;
class QtLiteDialog : public QDialog, private Ui::QtLiteDialog
{
    Q_OBJECT
    JsonRequest *modulesRequest;
    JsonRequest *featuresRequest;
    QString tag;
    QStringList _features;
    QPushButton *refreashFeaturesButton;

public:
    explicit QtLiteDialog(QWidget *parent = nullptr);

    QStringList features() const;

private slots:
    void on_tagsRequest_finished(QVariant data);
    void on_modulesRequest_finished(QVariant data);
    void on_featuresRequest_finished(QVariant data);
    void on_comboBoxModules_activated(const QString &arg1);
    void on_buttonBox_clicked(QAbstractButton *button);
};

#endif // QTLITEDIALOG_H
