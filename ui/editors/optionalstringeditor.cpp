#include "optionalstringeditor.h"

#include <QLineEdit>
#include <QCheckBox>
#include <QHBoxLayout>

OptionalStringEditor::OptionalStringEditor(QWidget *parent) : AbstractOptionEditor(parent)
{
    editor = new QWidget(this);
    auto l = new QHBoxLayout(editor);
    checkBox = new QCheckBox(this);
    checkBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    checkBox->setMinimumWidth(30);

    lineEdit = new QLineEdit(this);
    lineEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    lineEdit->setEnabled(false);

    l->addWidget(checkBox);
    l->addWidget(lineEdit);
    l->setMargin(0);
    l->setSpacing(0);
    editor->setLayout(l);

    connect(checkBox, &QCheckBox::toggled, lineEdit, &QWidget::setEnabled);
}

QVariant OptionalStringEditor::value() const
{
    if (!checkBox->isChecked())
        return QVariant();

    return lineEdit->text();
}

QWidget *OptionalStringEditor::createWidget()
{
    return editor;
}

void OptionalStringEditor::setValue(const QVariant &value)
{
    if (value == QVariant()) {
        lineEdit->clear();
        checkBox->setChecked(false);
    } else {
        lineEdit->setText(value.toString());
        checkBox->setChecked(true);
    }
}
