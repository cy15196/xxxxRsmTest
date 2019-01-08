#include "vector_item_delegate.h"
#include "fipsvectoredit.h"
vector_item_delegate::vector_item_delegate(QObject *parent)
    :QStyledItemDelegate(parent)
{

}

QWidget *vector_item_delegate::createEditor(QWidget *parent, const QStyleOptionViewItem &/*option*/, const QModelIndex &/*index*/) const
{
    FipsVectorEdit *vecEdit=new FipsVectorEdit(parent);
    //vecEdit->setValuesByString(index.data().toString());
    return vecEdit;
}

void vector_item_delegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    FipsVectorEdit *vecEdit =dynamic_cast<FipsVectorEdit *>(editor);
    if(vecEdit)
        vecEdit->setValuesByString(index.data().toString());
}

void vector_item_delegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    FipsVectorEdit *vecEdit =dynamic_cast<FipsVectorEdit *>(editor);
    if(vecEdit)
        model->setData(index,vecEdit->getValuesByString());
}
