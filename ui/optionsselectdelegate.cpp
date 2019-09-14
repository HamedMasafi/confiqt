#include "global.h"
#include "optionsselectdelegate.h"

#include <QToolButton>
#include <QDebug>
#include <option.h>

#include <editors/multistringeditor.h>
#include <editors/optionbooleditor.h>
#include <editors/multistringselector.h>
#include <editors/stringeditor.h>
#include <editors/pathselecteditor.h>
#include <editors/comboselector.h>
#include <editors/voideditor.h>
#include <editors/optionalstringeditor.h>

OptionsSelectDelegate::OptionsSelectDelegate(QObject *parent) : QItemDelegate(parent)
{
}

QWidget *OptionsSelectDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option)
    auto o = index.data(DataRole).value<Option*>();
    AbstractOptionEditor *editor = nullptr;
    auto dropDown = o->dropDown();

    switch (o->type()) {
    case Option::Unknown:
        Q_UNREACHABLE();
        break;
    case Option::Bool:
        editor = new OptionBoolEditor(parent);
        break;
    case Option::Enum:
        editor = new ComboSelector(parent);
        break;
    case Option::String:
        if (o->name().endsWith("dir"))
            editor = new PathSelectEditor(parent);
        else
            editor = new StringEditor(parent);
        break;
    case Option::OptionalString:
        editor = new OptionalStringEditor(parent);
        break;
    case Option::AddString:
        if (dropDown == QVariant())
            editor = new MultiStringEditor(parent);
        else
            editor = new MultiStringSelector(parent);
        break;
    case Option::Void:
        editor = new VoidEditor(parent);
        break;
    }

    editor->setDropDown(dropDown);
    return editor;
}

void OptionsSelectDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    auto w = dynamic_cast<AbstractOptionEditor*>(editor);
    QVariant data = index.data(Qt::EditRole);
    w->setValue(data);
}

void OptionsSelectDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    auto w = dynamic_cast<AbstractOptionEditor*>(editor);
    QVariant data = w->value();
    model->setData(index, w->value(), Qt::EditRole);
    model->setData(index, w->text(), Qt::DisplayRole);
}
