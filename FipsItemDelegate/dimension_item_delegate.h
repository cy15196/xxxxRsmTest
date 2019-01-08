/**
 * @file    dimension_item_delegate.h
 * @brief   item delegete for dimension
 * @author  chl
 * @date    2017.6.19
 * @version 1.0.0
 * @license
 * @note
 */

#ifndef FIPS_DIMENSION_ITEM_DELEGATE_H
#define FIPS_DIMENSION_ITEM_DELEGATE_H

#include <QStyledItemDelegate>
#include <QStringList>
#include "fipsitemdelegate_global.h"

/**
 * @brief   namespace of company, Fips & zygk
 */
namespace Fips
{

/**
 * @class   DimensionItemDelegate
 * @brief   item delegete for dimension
 * @author  chl
 * @date    2017.6.19
 * @note    dimension turn :
 *              Kg
 *              m
 *              s
 *              K
 *              A
 *              mol
 *              cd
 *          only create one, such as :
 *              Fips::DimensionItemDelegate *d = new Fips::DimensionItemDelegate(this);
 *              ui->tableWidget->setItemDelegateForColumn(0, d);
 */
class FIPSITEMDELEGATESHARED_EXPORT DimensionItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit DimensionItemDelegate(QObject *parent = FIPS_NULLPTR);
    ~DimensionItemDelegate();

    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const;

private:
    QStringList dimentionList;
};

}

#endif // FIPS_DIMENSION_ITEM_DELEGATE_H
