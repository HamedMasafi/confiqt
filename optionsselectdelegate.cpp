#include "global.h"
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
    Option::Type type = static_cast<Option::Type>(index.data(Wizard::TypeRole).toInt());
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
    if (w->type() != Option::Void)
        model->setData(index, w->value(), Qt::DisplayRole);
}

QSize OptionsSelectDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Option::Type type = static_cast<Option::Type>(index.data(Wizard::TypeRole).toInt());
    QSize size;// = QItemDelegate::sizeHint(option, index);
    size.setWidth(20);
    size.setHeight(120);
    return size;
}
