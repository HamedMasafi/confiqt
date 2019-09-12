#include "optionbooleditor.h"

OptionBoolEditor::OptionBoolEditor()
{
    setTristate(true);
}

QVariant OptionBoolEditor::value() const
{
    return checkState();
}

void OptionBoolEditor::setValue(const QVariant &value)
{
    setCheckState(value.value<Qt::CheckState>());
}
