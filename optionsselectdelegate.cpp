#include "optioneditwidget.h"
#include "optionsselectdelegate.h"
#include "wizard.h"

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
    QVariant::Type type = static_cast<QVariant::Type>(index.data(Wizard::TypeRole).toInt());
    QVariant dropDown = index.data(Wizard::DropDownRole);
    QVariant data = index.data(Qt::DisplayRole);
    qDebug()<<"Type is"<<type<<index.data(Wizard::TypeRole) << data;
    w->setType(type);
    w->setDropDown(dropDown);
    w->setValue(data);
}

void OptionsSelectDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    auto w = qobject_cast<OptionEditWidget*>(editor);
    if (w->type() != QVariant::Invalid)
        model->setData(index, w->value(), Qt::DisplayRole);
}
