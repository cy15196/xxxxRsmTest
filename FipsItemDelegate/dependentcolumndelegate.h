#ifndef DEPENDENTCOLUMNDELEGATE_H
#define DEPENDENTCOLUMNDELEGATE_H

#include <QStyledItemDelegate>
#include <QVariant>

namespace Fips {

/**
*@class:DependentColumnDelegate
*@brief:a class that provide delegate for
*          dependence between two columns
*@author:cyy
*@date:2017.07.28
*@note:the src column is depending on the dst column
*/


class DependentColumnDelegate:public QStyledItemDelegate
{
public:
   explicit DependentColumnDelegate(QObject*parent=0);
    ~DependentColumnDelegate();

    /***************reimplement**********************************/
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

    /***************methods***************************************/

    void addDependent(int src,int dst,QMap<QString,QStyledItemDelegate*> delegateMap);

    void removeDependent(int src,int dst);

    QMap<QPair<int, int>, QMap<QString, QStyledItemDelegate *> > getDependentMap() const;

private:
    QMap<QPair<int,int>,QMap<QString,QStyledItemDelegate*> > dependentMap;
    QMap<int,int> dependentColumnMap;
};

}

#endif // DEPENDENTCOLUMNDELEGATE_H
