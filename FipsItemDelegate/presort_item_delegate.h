#ifndef PRESORTITEMDELEGATE_H
#define PRESORTITEMDELEGATE_H

#include "fipsitemdelegate_global.h"

#include <QStyledItemDelegate>
#include <QComboBox>

namespace Fips {


template <typename _valueType_>
class FIPSITEMDELEGATESHARED_EXPORT PresortItemDelegate:public QStyledItemDelegate
{
public:
   explicit PresortItemDelegate(QObject*parent=0);
    ~PresortItemDelegate();
   QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
   void setEditorData(QWidget *editor, const QModelIndex &index) const;
   void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

   void addItemPair(const QString&showText,const _valueType_& value);
   void addItemPair(const QPair<QString,_valueType_>&itemPair);

   bool removeItemPairByText(const QString&showText,bool removeAll=false);
   bool removeItemPairByValue(const _valueType_&value,bool removeAll=false);
   void removeItemPair(const QPair<QString, _valueType_>&itemPair,bool removeAll=false);

   QPair<QString, _valueType_> findItemPairByValue(const _valueType_&value) const;
   QPair<QString, _valueType_> findItemPairByText(const QString&text) const;

   QList<QPair<QString, _valueType_> > getPairList() const;
   void setPairList(const QList<QPair<QString, _valueType_> > &value);

   bool getEditable() const;
   void setEditable(bool value);

private:
   QList<QPair<QString,_valueType_> > pairList;
   bool editable;
};

template<typename _valueType_>
PresortItemDelegate<_valueType_>::PresortItemDelegate(QObject *parent)
    :QStyledItemDelegate(parent),editable(false)
{

}

template<typename _valueType_>
PresortItemDelegate<_valueType_>::~PresortItemDelegate()
{

}

template<typename _valueType_>
QWidget* PresortItemDelegate<_valueType_>::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QComboBox *comboBox=new QComboBox(parent);
    for(int i=0;i<pairList.count();i++) {
        QPair<QString,_valueType_> itemPair=pairList[i];
        comboBox->addItem(itemPair.first);
    }
    comboBox->setEditable(editable);
    return comboBox;
}

template<typename _valueType_>
void PresortItemDelegate<_valueType_>::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QComboBox*comboBox=dynamic_cast<QComboBox*>(editor);
    QString showText=findItemPairByValue(index.data().value<_valueType_>()).first;
    comboBox->setCurrentIndex(comboBox->findText(showText));
}

template<typename _valueType_>
void PresortItemDelegate<_valueType_>::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox*comboBox=dynamic_cast<QComboBox*>(editor);
    QString showText=comboBox->currentText();
    model->setData(index,findItemPairByText(showText).second);
}

template<typename _valueType_>
void PresortItemDelegate<_valueType_>::addItemPair(const QString &showText, const _valueType_ &value)
{
    QPair<QString,_valueType_> itemPair;
    itemPair.first=showText;
    itemPair.second=value;
    addItemPair(itemPair);
}

template<typename _valueType_>
void PresortItemDelegate<_valueType_>::addItemPair(const QPair<QString, _valueType_> &itemPair)
{
    pairList.append(itemPair);
}

template<typename _valueType_>
bool PresortItemDelegate<_valueType_>::removeItemPairByText(const QString &showText, bool removeAll)
{
    bool isSuccess=false;
    for(int i=0;i<pairList.count();i++) {
        QPair<QString,_valueType_> itemPair=pairList[i];
        if(itemPair.first==showText)
        {
            pairList.removeOne(itemPair);
            isSuccess=true;
            if(!removeAll)
                break;
        }
    }

    return isSuccess;
}

template<typename _valueType_>
bool PresortItemDelegate<_valueType_>::removeItemPairByValue(const _valueType_ &value, bool removeAll)
{
    bool isSuccess=false;
    for(int i=0;i<pairList.count();i++) {
        QPair<QString,_valueType_> itemPair=pairList[i];
        if(itemPair.second==value)
        {
            pairList.removeOne(itemPair);
            isSuccess=true;
            if(!removeAll)
                break;
        }
    }
    return isSuccess;
}

template<typename _valueType_>
void PresortItemDelegate<_valueType_>::removeItemPair(const QPair<QString, _valueType_> &itemPair, bool removeAll)
{
    if(removeAll)
        pairList.removeAll(itemPair);
    else
        pairList.removeOne(itemPair);
}

template<typename _valueType_>
QPair<QString, _valueType_> PresortItemDelegate<_valueType_>::findItemPairByValue(const _valueType_ &value) const
{
    for(int i=0;i<pairList.count();i++) {
        QPair<QString,_valueType_> itemPair=pairList[i];
        if(itemPair.second==value)
        {
            return itemPair;
        }
    }
    return QPair<QString,_valueType_>();
}

template<typename _valueType_>
QPair<QString, _valueType_> PresortItemDelegate<_valueType_>::findItemPairByText(const QString &text) const
{
    for(int i=0;i<pairList.count();i++) {
        QPair<QString,_valueType_> itemPair=pairList[i];
        if(itemPair.first==text)
        {
            return itemPair;
        }
    }
    return QPair<QString,_valueType_>();
}

template<typename _valueType_>
QList<QPair<QString, _valueType_> > PresortItemDelegate<_valueType_>::getPairList() const
{
    return pairList;
}

template<typename _valueType_>
void PresortItemDelegate<_valueType_>::setPairList(const QList<QPair<QString, _valueType_> > &value)
{
    pairList = value;
}

template<typename _valueType_>
bool PresortItemDelegate<_valueType_>::getEditable() const
{
    return editable;
}

template<typename _valueType_>
void PresortItemDelegate<_valueType_>::setEditable(bool value)
{
    editable = value;
}


}

#endif // PRESORTITEMDELEGATE_H
