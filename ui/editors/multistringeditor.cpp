#include "multistringeditor.h"

#include <QPlainTextEdit>

MultiStringEditor::MultiStringEditor() : CustomComboBox()
{
    editor = new QPlainTextEdit(this);
    editor->hide();
    connect(editor, &QPlainTextEdit::textChanged,
            this, &MultiStringEditor::editor_textChanged);
}

void MultiStringEditor::editor_textChanged()
{
    setText(editor->toPlainText().replace("\n", ", "));
}

QWidget *MultiStringEditor::createEditor()
{
    return editor;
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
    }
}
