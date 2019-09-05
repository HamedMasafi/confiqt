#include "global.h"
#include "optioneditwidget.h"
#include "optionsselectdelegate.h"

#include <QDebug>

OptionsSelectDelegate::OptionsSelectDelegate(QObject *parent) : QItemDelegate(parent)
{
    setClipping(true);
}

QWidget *OptionsSelectDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
    return new OptionEditWidget(parent);
}

void OptionsSelectDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    auto w = qobject_cast<OptionEditWidget*>(editor);
    Option::Type type = static_cast<Option::Type>(index.data(TypeRole).toInt());
    QVariant dropDown = index.data(DropDownRole);
    QVariant data = index.data(Qt::EditRole);

    if (type == Option::AddString)
        qDebug() << dropDown;

    w->setType(type);
    w->setDropDown(dropDown);
    w->setValue(data);
}

void OptionsSelectDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    auto w = qobject_cast<OptionEditWidget*>(editor);
    QVariant data = w->value();
    model->setData(index, w->value(), Qt::EditRole);
    if (data.type() == QVariant::List) {
        auto list = data.toList();
        QString text;
        foreach (QVariant v, list) {
            if (!text.isEmpty())
                text.append(", ");
            text.append(v.toString());
        }
        model->setData(index, text, Qt::DisplayRole);
    } else {
        model->setData(index, data, Qt::DisplayRole);
    }
}
