#ifndef OPTIONSSELECTDELEGATE_H
#define OPTIONSSELECTDELEGATE_H

#include <QItemDelegate>



class OptionsSelectDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    OptionsSelectDelegate(QObject *parent = nullptr);

    // QAbstractItemDelegate interface
public:
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

    // QAbstractItemDelegate interface
public:
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // OPTIONSSELECTDELEGATE_H
