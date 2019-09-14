#include "multistringeditor.h"

#include <QPlainTextEdit>

MultiStringEditor::MultiStringEditor(QWidget *parent) : AbstractOptionEditor(parent), editor(nullptr)
{
    comboBox = new CustomComboBox(this);
    editor = new QPlainTextEdit;
    editor->hide();
    comboBox->setPopup(editor);
    connect(editor, &QPlainTextEdit::textChanged,
            this, &MultiStringEditor::editor_textChanged);
}

void MultiStringEditor::editor_textChanged()
{
    comboBox->setText(editor->toPlainText().replace("\n", ", "));
}

QWidget *MultiStringEditor::createWidget()
{
    return comboBox;
}

QVariant MultiStringEditor::value() const
{
    QVariantList v;
    foreach (QString s, editor->toPlainText().split('\n'))
        if (!s.isEmpty())
            v.append(s);
    return v;
}

void MultiStringEditor::setValue(const QVariant &value)
{
    if (value.type() == QVariant::List) {
        QString text;
        auto list = value.toList();
        foreach (QVariant v, list)
            if (v.type() == QVariant::String) {
                if (!text.isEmpty())
                    text.append('\n');
                text.append(v.toString());
            }
        editor->setPlainText(text);
    } else {
        editor->clear();
    }
}
