/**
 * @file    color_item_delegate.h
 * @brief   item delegete for color
 * @author  chl
 * @date    2017.7.26
 * @version 1.1.0
 * @license
 * @note
 */

#ifndef FIPS_LINE_COLOR_ITEM_DELEGATE_H
#define FIPS_LINE_COLOR_ITEM_DELEGATE_H

#include <QStyledItemDelegate>
#include "fipsitemdelegate_global.h"

/**
 * @brief   namespace of company, Fips & zygk
 */
namespace Fips
{

/**
 * @class   ColorItemDelegate
 * @brief   item delegete for color
 * @author  chl
 * @date    2017.7.26
 * @note    only create one, such as :
 *              Fips::ColorItemDelegate *d = new Fips::ColorItemDelegate(this);
 *              ui->tableWidget->setItemDelegateForColumn(0, d);
 *          if alpha is :
 *              false   #RRGGBB
 *              true    #AARRGGBB
 */
class FIPSITEMDELEGATESHARED_EXPORT LineColorItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit LineColorItemDelegate(QObject *parent = FIPS_NULLPTR);
    LineColorItemDelegate(bool hasAlpha, QObject *parent = FIPS_NULLPTR);
    ~LineColorItemDelegate();

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

private:
    bool alpha;
};

}

#endif // FIPS_LINE_COLOR_ITEM_DELEGATE_H
