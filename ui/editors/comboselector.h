#ifndef COMBOSELECTOR_H
#define COMBOSELECTOR_H

#include "abstractoptioneditor.h"

class QComboBox;
class ComboSelector : public AbstractOptionEditor
{
    Q_OBJECT
    QComboBox *editor;

public:
    ComboSelector(QWidget *parent = nullptr);

public:
    QVariant value() const override;
    void setValue(const QVariant &value) override;
    void setDropDown(const QStringList &list) override;
    QWidget *createWidget() override;
};

#endif // COMBOSELECTOR_H
