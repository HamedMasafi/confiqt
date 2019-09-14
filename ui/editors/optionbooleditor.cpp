#include "optionbooleditor.h"

OptionBoolEditor::OptionBoolEditor(QWidget *parent) : AbstractOptionEditor(parent)
{
    editor = new QCheckBox(this);
    editor->setTristate(true);
    connect(editor, &QCheckBox::stateChanged,
            this, &OptionBoolEditor::me_stateChanged);
}

QVariant OptionBoolEditor::value() const
{
    return editor->checkState();
}

void OptionBoolEditor::setValue(const QVariant &value)
{
    if (value == QVariant())
        editor->setCheckState(Qt::PartiallyChecked);
    else
        editor->setCheckState(value.value<Qt::CheckState>());
    me_stateChanged(editor->checkState());
}

void OptionBoolEditor::me_stateChanged(int state)
{
    switch (state) {
    case Qt::Checked:
        editor->setText("true");
        break;
    case Qt::Unchecked:
        editor->setText("false");
        break;
    case Qt::PartiallyChecked:
        editor->setText("<not set>");
        break;
    }
}

QWidget *OptionBoolEditor::createWidget()
{
    return editor;
}

QString OptionBoolEditor::text() const
{
    switch (editor->checkState()) {
    case Qt::Checked:
        return "true";
    case Qt::Unchecked:
        return "false";
    case Qt::PartiallyChecked:
        return QString();
    }
}
