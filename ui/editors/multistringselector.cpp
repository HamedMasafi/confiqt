#include "multistringselector.h"

#include <QListWidget>

MultiStringSelector::MultiStringSelector()
{
    editor = new QListWidget(this);
    editor->setWindowFlag(Qt::Popup, true);
    editor->hide();
    connect(editor, &QListWidget::itemChanged,
            this, &MultiStringSelector::editor_itemChanged);
}

void MultiStringSelector::editor_itemChanged(QListWidgetItem *item)
{
    QString s;
    for (int i = 0; i < editor->count(); ++i) {
        if (editor->item(i)->checkState() == Qt::Checked) {
            if (!s.isEmpty())
                s.append(", ");
            s.append(editor->item(i)->text());
        }
    }
    setText(s);
}

QVariant MultiStringSelector::value() const
{

}

void MultiStringSelector::setValue(const QVariant &value)
{
    if (value.type() == QVariant::List) {
        QString text;
        auto list = value.toList();
        foreach (QVariant v, list) {
            auto item = new QListWidgetItem(editor);
            item->setText(v.toString());
            item->setCheckState(Qt::Unchecked);
        }
    }
}

QWidget *MultiStringSelector::createEditor()
{
    return editor;
}
