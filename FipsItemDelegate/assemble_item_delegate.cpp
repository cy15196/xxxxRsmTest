#include "assemble_item_delegate.h"

#define applyItemFunction(functionString)\
    QPair<int,int> indexPair;\
    indexPair.first=index.row();\
    indexPair.second=index.column();\
    if(itemDelegateMap.keys().contains(indexPair))\
        return itemDelegateMap[indexPair]->functionString;\
    else if(columnDelegateMap.keys().contains(indexPair.second))\
        return columnDelegateMap[indexPair.second]->functionString;\
    else if(columnDelegateMap.keys().contains(indexPair.first))\
        return columnDelegateMap[indexPair.first]->functionString;\
    return QStyledItemDelegate::functionString;\





Assemble_item_delegate::Assemble_item_delegate(QObject *parent)
    :QStyledItemDelegate(parent)
{

}

void Assemble_item_delegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    //applyItemFunction(paint(painter, option, index));
    QPair<int,int> indexPair;
    indexPair.first=index.row();
    indexPair.second=index.column();
    if(itemDelegateMap.keys().contains(indexPair))
        return itemDelegateMap[indexPair]->paint(painter, option, index);
    else if(columnDelegateMap.keys().contains(indexPair.second))
        return columnDelegateMap[indexPair.second]->paint(painter, option, index);
    else if(rowDelegateMap.keys().contains(indexPair.first))
        return rowDelegateMap[indexPair.first]->paint(painter, option, index);
    return QStyledItemDelegate::paint(painter, option, index);
}

QWidget *Assemble_item_delegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    applyItemFunction(createEditor(parent,option,index));
}

void Assemble_item_delegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    applyItemFunction(setEditorData(editor,index));
}

void Assemble_item_delegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    applyItemFunction(setModelData(editor,model,index));
}

bool Assemble_item_delegate::addStyleItemDelegateForSingleItem(const int &row, const int &column, QStyledItemDelegate *delegate)
{
    QPair<int,int> indexPair;
    indexPair.first=row;
    indexPair.second=column;

    itemDelegateMap[indexPair]=delegate;
    return true;
}

bool Assemble_item_delegate::addStyleItemDelegateForSingleItem(const QModelIndex &index, QStyledItemDelegate *delegate)
{
    return addStyleItemDelegateForSingleItem(index.row(),index.column(),delegate);
}

bool Assemble_item_delegate::addStyleItemDelegateForItemList(const QModelIndexList &indexList, QStyledItemDelegate *delegate)
{
    foreach (QModelIndex index, indexList) {
        addStyleItemDelegateForSingleItem(index,delegate);
    }
    return true;
}

bool Assemble_item_delegate::addStyleItemDelegateForRow(const int &row,  QStyledItemDelegate *delegate)
{
    rowDelegateMap[row]=delegate;
    return true;
}

bool Assemble_item_delegate::addStyleItemDelegateForColumn(const int &column, QStyledItemDelegate *delegate)
{
    columnDelegateMap[column]=delegate;
    return true;
}
