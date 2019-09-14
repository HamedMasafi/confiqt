#include "comboselector.h"

#include <QComboBox>

ComboSelector::ComboSelector(QWidget *parent) : AbstractOptionEditor(parent)
{
    editor = new QComboBox(this);
}

QVariant ComboSelector::value() const
{
    return editor->currentText();
}

void ComboSelector::setValue(const QVariant &value)
{
    if (value == QVariant())
        editor->setCurrentIndex(-1);
    else
        editor->setCurrentText(value.toString());
}

void ComboSelector::setDropDown(const QStringList &list)
{
    editor->addItems(list);
}

QWidget *ComboSelector::createWidget()
{
    return editor;
}
