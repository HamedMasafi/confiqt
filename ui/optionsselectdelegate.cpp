#include "global.h"
#include "optioneditwidget.h"
#include "optionsselectdelegate.h"

#include <QDebug>

OptionsSelectDelegate::OptionsSelectDelegate(QObject *parent) : QItemDelegate(parent)
{
}

QWidget *OptionsSelectDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
    qDebug() << Q_FUNC_INFO;
    return new OptionEditWidget(parent);

//    auto w = new QWidget(parent);
//    auto l = new QHBoxLayout(w);
//    l->addWidget(new QWidget);
//    l->addWidget(new QToolButton);
//    w->setLayout(l);
//    return w;
}

void OptionsSelectDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    auto w = qobject_cast<OptionEditWidget*>(editor);
    auto o = index.data(DataRole).value<Option*>();

    w->setOption(o);

//    Option::Type type = static_cast<Option::Type>(index.data(TypeRole).toInt());
//    QVariant dropDown = index.data(DropDownRole);
    QVariant data = index.data(Qt::EditRole);

//    w->setType(type);
//    w->setDropDown(dropDown);
    w->setValue(data);
}

void OptionsSelectDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    auto w = qobject_cast<OptionEditWidget*>(editor);
    QVariant data = w->value();
    model->setData(index, w->value(), Qt::EditRole);
    w->option()->setValue(w->value());
    model->setData(index, w->option()->displayText(), Qt::DisplayRole);
//    if (data.type() == QVariant::List) {
//        auto list = data.toList();
//        QString text;
//        foreach (QVariant v, list) {
//            if (!text.isEmpty())
//                text.append(", ");
//            text.append(v.toString());
//        }
//        model->setData(index, text, Qt::DisplayRole);
//    } else {
//        model->setData(index, data, Qt::DisplayRole);
//    }
}
