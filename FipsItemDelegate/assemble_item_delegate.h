#ifndef ASSEMBLE_ITEM_DELEGATE_H
#define ASSEMBLE_ITEM_DELEGATE_H

#include "fipsitemdelegate_global.h"
#include <QStyledItemDelegate>
#include <QMap>
#include <QPair>

///try to write a class that can set delegate for a signal item at any row and column

class FIPSITEMDELEGATESHARED_EXPORT Assemble_item_delegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit Assemble_item_delegate(QObject*parent=0);
    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const;
    QWidget *createEditor(QWidget *parent,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor,
                      QAbstractItemModel *model,
                      const QModelIndex &index) const;

    bool addStyleItemDelegateForSingleItem(const int& row,const int& column, QStyledItemDelegate*delegate);
    bool addStyleItemDelegateForSingleItem(const QModelIndex&index, QStyledItemDelegate*delegate);
    bool addStyleItemDelegateForItemList(const QModelIndexList&indexList, QStyledItemDelegate*delegate);

    bool addStyleItemDelegateForRow(const int& row, QStyledItemDelegate*delegate);
    bool addStyleItemDelegateForColumn(const int& column, QStyledItemDelegate*delegate);


private:
    ///map that store the delegate for each item that set by user
    QMap<QPair<int,int> ,QStyledItemDelegate* > itemDelegateMap;
    QMap<int  ,QStyledItemDelegate* > columnDelegateMap;
    QMap<int  ,QStyledItemDelegate* > rowDelegateMap;
};

#endif // ASSEMBLE_ITEM_DELEGATE_H
