#include "pathselecteditor.h"

#include <QFileDialog>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QToolButton>

PathSelectEditor::PathSelectEditor(QWidget *parent) : AbstractOptionEditor(parent)
{
    editor = new QWidget(this);
    auto l = new QHBoxLayout(editor);
    auto selectButton = new QToolButton(this);
    selectButton->setText("...");
    selectButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);

    lineEdit = new QLineEdit(this);
    lineEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    l->addWidget(lineEdit);
    l->addWidget(selectButton);
    l->setMargin(0);
    l->setSpacing(0);
    editor->setLayout(l);

    connect(selectButton, &QAbstractButton::clicked,
            this, &PathSelectEditor::button_clicked);
}

QVariant PathSelectEditor::value() const
{
    return lineEdit->text();
}

void PathSelectEditor::setValue(const QVariant &value)
{
    if (value == QVariant())
        lineEdit->clear();
    else
        lineEdit->setText(value.toString());
}

void PathSelectEditor::button_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this, QString(), lineEdit->text());
    if (path != QString())
        lineEdit->setText(path);
}

QWidget *PathSelectEditor::createWidget()
{
    return editor;
}
