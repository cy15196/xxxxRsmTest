/**
 * @file    list_item_delegate.h
 * @brief   item delegete for comboBox
 * @author  chl
 * @date    2017.6.19
 * @version 1.0.0
 * @license
 * @note    item delegete for comboBox
 *          data is a list
 */

#ifndef FIPS_LIST_ITEM_DELEGATE_H
#define FIPS_LIST_ITEM_DELEGATE_H

#include <QStyledItemDelegate>
#include <QStringList>
#include "fipsitemdelegate_global.h"

/**
 * @brief   namespace of company, Fips & zygk
 */
namespace Fips
{

/**
 * @class   ListItemDelegate
 * @brief   item delegete for comboBox
 * @author  chl
 * @date    2017.6.19
 * @note    data is a list
 *          create one and give it an option list, such as :
 *              Fips::ListItemDelegate *d = new Fips::ListItemDelegate(this);
 *              d->setOptions(QStringList() << "A" << "B" << "C" << "D");
 *              ui->tableWidget->setItemDelegateForColumn(0, d);
 */
class FIPSITEMDELEGATESHARED_EXPORT ListItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit ListItemDelegate(QObject *parent = FIPS_NULLPTR);
    ~ListItemDelegate();

    QWidget *createEditor(QWidget *parent,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor,
                      QAbstractItemModel *model,
                      const QModelIndex &index) const;

    /**
     * @brief   options actions
     */
    void setOptions(const QStringList &options)
    { valueList = options; }

    void clearOptions()
    { valueList.clear(); }

    void addOptions(const QStringList &options)
    { valueList << options; }

    void addOption(const QString &option)
    { valueList << option; }

    void removeOption(const QString &option)
    { valueList.removeAll(option); }

    void removeOption(int index)
    { valueList.removeAt(index); }

    int optionCount() const
    { return valueList.count(); }

    QString getOption(int index) const
    { return valueList.value(index); }

    /**
     * @brief   set ComboBox Editable
     * @param   bool b
     *              true    can edit
     *              false   can not edit
     */
    void setComboBoxEditable(bool b)
    { editable = b; }

    bool comboBoxEditable()
    { return editable; }

private:
    QStringList valueList;
    bool editable;
};

}

#endif // FIPS_LIST_ITEM_DELEGATE_H
