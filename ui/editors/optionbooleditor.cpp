#include "optionbooleditor.h"

OptionBoolEditor::OptionBoolEditor() : QCheckBox()
{
    setTristate(true);
    connect(this, &QCheckBox::stateChanged,
            this, &OptionBoolEditor::me_stateChanged);
}

QVariant OptionBoolEditor::value() const
{
    return checkState();
}

void OptionBoolEditor::setValue(const QVariant &value)
{
    setCheckState(value.value<Qt::CheckState>());
}

void OptionBoolEditor::me_stateChanged(int state)
{
    switch (state) {
    case Qt::Checked:
        setText("true");
        break;
    case Qt::Unchecked:
        setText("false");
        break;
    case Qt::PartiallyChecked:
        setText("<not set>");
        break;
    }
}
