#ifndef VECTOR_ITEM_DELEGATE_H
#define VECTOR_ITEM_DELEGATE_H

#include "fipsitemdelegate_global.h"
#include <QStyledItemDelegate>



class FIPSITEMDELEGATESHARED_EXPORT vector_item_delegate : public QStyledItemDelegate
{
public:
    explicit vector_item_delegate(QObject*parent=0);

    QWidget *createEditor(QWidget *parent,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor,
                      QAbstractItemModel *model,
                      const QModelIndex &index) const;
};

#endif // VECTOR_ITEM_DELEGATE_H
