#include "voideditor.h"
#include <QCheckBox>

VoidEditor::VoidEditor(QWidget *parent) : AbstractOptionEditor(parent)
{
    editor = new QCheckBox(this);
    editor->setTristate(false);
    connect(editor, &QCheckBox::toggled,
            this, &VoidEditor::me_toggled);
}

QVariant VoidEditor::value() const
{
    return editor->isChecked();
}

void VoidEditor::setValue(const QVariant &value)
{
    if (value.type() == QVariant::Bool)
        editor->setChecked(value.toBool());
    else
        editor->setChecked(false);
}

void VoidEditor::me_toggled(bool checked)
{
    if (checked)
        editor->setText("true");
    else
        editor->setText("");
}

QWidget *VoidEditor::createWidget()
{
    return editor;
}

QString VoidEditor::text() const
{
    return editor->text();
}
