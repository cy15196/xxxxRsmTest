#ifndef VALIDATORITEMDELEGATE_H
#define VALIDATORITEMDELEGATE_H

#include "fipsitemdelegate_global.h"
#include <QStyledItemDelegate>


class QValidator;

namespace Fips {


/**
*@class:ValidatorItemDelegate
*@brief:a delagete provide an editor with validator for table,list or other's item
*@author:cyy
*@date:2017.07.24
*@note:this delegate return a text editor for the item
*      and you can set an validator to it
*/


class FIPSITEMDELEGATESHARED_EXPORT  ValidatorItemDelegate:public QStyledItemDelegate
{
public:

    explicit ValidatorItemDelegate(QObject*parent=0);
    ~ValidatorItemDelegate();

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    QWidget *createEditor(QWidget *parent,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;
    QValidator *getValidtor() const;
    void setValidtor(QValidator *value);


    ///provide some defalut validType
    void applyDoubleValidtor();
    void applyIntValidtor();

private:

    QValidator*validtor;

};


}
#endif // VALIDATORITEMDELEGATE_H
