#ifndef MULTISTRINGSELECTOR_H
#define MULTISTRINGSELECTOR_H

#include "abstractoptioneditor.h"

class QListWidgetItem;
class QListWidget;
class CustomComboBox;
class MultiStringSelector : public AbstractOptionEditor
{
    Q_OBJECT
    QListWidget *editor;
    CustomComboBox *comboBox;

public:
    MultiStringSelector(QWidget *parent = nullptr);
    QVariant value() const override;
    void setValue(const QVariant &value) override;
    void setDropDown(const QStringList &list) override;
    QWidget *createWidget() override;
    QString text() const override;

private slots:
    void editor_itemChanged(QListWidgetItem *item);
};

#endif // MULTISTRINGSELECTOR_H
