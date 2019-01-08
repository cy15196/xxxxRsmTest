#include "list_item_delegate.h"
#include <QComboBox>

Fips::ListItemDelegate::ListItemDelegate(QObject *parent) :
    QStyledItemDelegate(parent),
    editable(false)
{

}

Fips::ListItemDelegate::~ListItemDelegate()
{

}

QWidget *Fips::ListItemDelegate::createEditor(QWidget *parent,
                                              const QStyleOptionViewItem &/*option*/,
                                              const QModelIndex &/*index*/) const
{
    QComboBox *comboBox = new QComboBox(parent);
    comboBox->addItems(valueList);
    comboBox->setEditable(editable);
    return comboBox;
}

void Fips::ListItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    dynamic_cast<QComboBox *>(editor)->setCurrentIndex(valueList.indexOf(index.data().toString()));
}

void Fips::ListItemDelegate::setModelData(QWidget *editor,
                                          QAbstractItemModel *model,
                                          const QModelIndex &index) const
{
    QString str = dynamic_cast<QComboBox *>(editor)->currentText();
    if (valueList.indexOf(str) < 0)
        return;
    model->setData(index, str);
}
