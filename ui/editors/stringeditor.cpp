#include "stringeditor.h"
#include <QLineEdit>

StringEditor::StringEditor(QWidget *parent) : AbstractOptionEditor(parent)
{
    editor = new QLineEdit(this);
}

QVariant StringEditor::value() const
{
    return editor->text();
}

QWidget *StringEditor::createWidget()
{
    return editor;
}

void StringEditor::reset()
{
    editor->clear();
}

void StringEditor::setValue(const QVariant &value)
{
    if (value == QVariant())
        editor->clear();
    else
        editor->setText(value.toString());
}
