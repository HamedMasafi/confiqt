#ifndef MULTISTRINGSELECTOR_H
#define MULTISTRINGSELECTOR_H

#include "abstractoptioneditor.h"
#include "customcombobox.h"

class QListWidgetItem;
class QListWidget;
class MultiStringSelector : public CustomComboBox, public AbstractOptionEditor
{
    Q_OBJECT
    QListWidget *editor;

public:
    MultiStringSelector();

    // AbstractOptionEditor interface
public:
    QVariant value() const;
    void setValue(const QVariant &value);
    void setDropDown(const QStringList &list);

protected:
    QWidget *createEditor();
private slots:
    void editor_itemChanged(QListWidgetItem *item);
};

#endif // MULTISTRINGSELECTOR_H
