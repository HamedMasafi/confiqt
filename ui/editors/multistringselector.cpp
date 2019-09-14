#include "multistringselector.h"

#include <QListWidget>
#include <customcombobox.h>

MultiStringSelector::MultiStringSelector(QWidget *parent) : AbstractOptionEditor(parent), editor(nullptr)
{
    comboBox = new CustomComboBox(this);
    editor = new QListWidget;
    editor->hide();
    comboBox->setPopup(editor);
    connect(editor, &QListWidget::itemChanged,
            this, &MultiStringSelector::editor_itemChanged);
}

void MultiStringSelector::editor_itemChanged(QListWidgetItem *item)
{
    Q_UNUSED(item)
    QString s;
    for (int i = 0; i < editor->count(); ++i) {
        if (editor->item(i)->checkState() == Qt::Checked) {
            if (!s.isEmpty())
                s.append(", ");
            s.append(editor->item(i)->text());
        }
    }
    comboBox->setText(s);
}

QWidget *MultiStringSelector::createWidget()
{
    return comboBox;
}

QVariant MultiStringSelector::value() const
{
    QVariantList list;
    for (int i = 0; i < editor->count(); ++i)
        if (editor->item(i)->checkState() == Qt::Checked)
            list.append(editor->item(i)->text());
    return list;
}

void MultiStringSelector::setValue(const QVariant &value)
{
    for (int i = 0; i < editor->count(); ++i)
        editor->item(i)->setCheckState(Qt::Unchecked);

    if (value.type() == QVariant::List) {
        QString text;
        auto list = value.toList();
        foreach (QVariant v, list) {
            auto l = editor->findItems(v.toString(), Qt::MatchFixedString);
            foreach (auto i, l) {
                i->setCheckState(Qt::Checked);
            }
        }
    }
}

void MultiStringSelector::setDropDown(const QStringList &list)
{
    foreach (QVariant v, list) {
        auto item = new QListWidgetItem(editor);
        item->setText(v.toString());
        item->setCheckState(Qt::Unchecked);
    }
}
