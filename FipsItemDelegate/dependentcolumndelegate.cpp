#include "dependentcolumndelegate.h"

namespace Fips {



DependentColumnDelegate::DependentColumnDelegate(QObject*parent)
    :QStyledItemDelegate(parent)
{

}

DependentColumnDelegate::~DependentColumnDelegate()
{

}

void DependentColumnDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(dependentColumnMap.keys().contains(index.column()))
    {
        int srcColumn=index.column();
        int dstColumn=dependentColumnMap[srcColumn];
        QVariant dependData = index.model()->index(index.row(),dstColumn,index.parent()).data();
        QStyledItemDelegate* delegate=(dependentMap[QPair<int,int>(srcColumn,dstColumn)])[dependData.toString()];
        if(delegate)
            return delegate->paint(painter,option,index);
    }
    return QStyledItemDelegate::paint(painter,option,index);
}

QWidget *DependentColumnDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(dependentColumnMap.keys().contains(index.column()))
    {
        int srcColumn=index.column();
        int dstColumn=dependentColumnMap[srcColumn];
        QVariant dependData = index.model()->index(index.row(),dstColumn,index.parent()).data();
        QStyledItemDelegate* delegate=(dependentMap[QPair<int,int>(srcColumn,dstColumn)])[dependData.toString()];
        if(delegate)
            return delegate->createEditor(parent,option,index);
    }
    return QStyledItemDelegate::createEditor(parent,option,index);
}

void DependentColumnDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if(dependentColumnMap.keys().contains(index.column()))
    {
        int srcColumn=index.column();
        int dstColumn=dependentColumnMap[srcColumn];
        QVariant dependData = index.model()->index(index.row(),dstColumn,index.parent()).data();
        QStyledItemDelegate* delegate=(dependentMap[QPair<int,int>(srcColumn,dstColumn)])[dependData.toString()];
        if(delegate)
            return delegate->setEditorData(editor,index);
    }
    return QStyledItemDelegate::setEditorData(editor,index);
}

void DependentColumnDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if(dependentColumnMap.keys().contains(index.column()))
    {
        int srcColumn=index.column();
        int dstColumn=dependentColumnMap[srcColumn];
        QVariant dependData = index.model()->index(index.row(),dstColumn,index.parent()).data();
        QStyledItemDelegate* delegate=(dependentMap[QPair<int,int>(srcColumn,dstColumn)])[dependData.toString()];
        if(delegate)
            return delegate->setModelData(editor,model,index);

    }
    return QStyledItemDelegate::setModelData(editor,model,index);
}





void DependentColumnDelegate::addDependent(int src, int dst, QMap<QString, QStyledItemDelegate *> delegateMap)
{
    dependentMap[QPair<int,int>(src,dst)]=delegateMap;
    dependentColumnMap[src]=dst;
}

void DependentColumnDelegate::removeDependent(int src, int dst)
{
    dependentMap.remove(QPair<int,int>(src,dst));
    dependentColumnMap.remove(src);
}

QMap<QPair<int, int>, QMap<QString, QStyledItemDelegate *> > DependentColumnDelegate::getDependentMap() const
{
    return dependentMap;
}


}
