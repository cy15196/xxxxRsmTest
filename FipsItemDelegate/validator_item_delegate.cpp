#include "validator_item_delegate.h"

#include <QLineEdit>
#include <QDoubleValidator>
#include <QIntValidator>

namespace Fips {


ValidatorItemDelegate::ValidatorItemDelegate(QObject *parent)
    :QStyledItemDelegate(parent)
{

}

ValidatorItemDelegate::~ValidatorItemDelegate()
{
    if(validtor)
        delete validtor;
}

void ValidatorItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
   dynamic_cast<QLineEdit*>(editor)->setText(index.data().toString());
}

QWidget *ValidatorItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QLineEdit*editor=new QLineEdit(parent);
    editor->setValidator(validtor);
    return editor;
}

QValidator *ValidatorItemDelegate::getValidtor() const
{
    return validtor;
}

void ValidatorItemDelegate::setValidtor(QValidator *value)
{
    validtor = value;
}

void ValidatorItemDelegate::applyDoubleValidtor()
{
    validtor=new QDoubleValidator(this);
}

void ValidatorItemDelegate::applyIntValidtor()
{
    validtor=new QIntValidator(this);
}

}
