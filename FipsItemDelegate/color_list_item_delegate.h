/**
 * @file    color_list_item_delegate.h
 * @brief   item delegete for color's comboBox
 * @author  chl
 * @date    2017.6.20
 * @version 1.0.0
 * @license
 * @note    item delegete for color's comboBox
 *          data is a color list
 */

#ifndef COLOR_LIST_ITEM_DELEGATE_H
#define COLOR_LIST_ITEM_DELEGATE_H

#include <QStyledItemDelegate>
#include "fipsitemdelegate_global.h"

/**
 * @brief   namespace of company, Fips & zygk
 */
namespace Fips
{

/**
 * @brief   namespace of ColorListItemDelegate private
 */
namespace __colorlstitemdelegateprivate_
{

/**
 * @class   ColorItemDelegate
 * @brief   item delegete for comboBox item
 * @author  chl
 * @date    2017.6.20
 * @note    only to show color from text "#XXXXXX"
 */
class ColorItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit ColorItemDelegate(QObject *parent = FIPS_NULLPTR);
    ~ColorItemDelegate();

    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const;
};

}

/**
 * @class   ColorListItemDelegate
 * @brief   item delegete for color's comboBox
 * @author  chl
 * @date    2017.6.20
 * @note    data is a list
 *          create one and give it a color list or not, such as :
 *              Fips::ColorListItemDelegate *d = new Fips::ColorListItemDelegate(this);
 *              ui->tableWidget->setItemDelegateForColumn(0, d);
 */
class FIPSITEMDELEGATESHARED_EXPORT ColorListItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit ColorListItemDelegate(QObject *parent = FIPS_NULLPTR);
    ~ColorListItemDelegate();

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

    /**
     * @brief   colors actions
     */
    void setColorList(const QStringList &colors)
    { colorList = colors; }

    void clearColors()
    { colorList.clear(); }

    void addColors(const QString &colors)
    { colorList << colors; }

    void addColor(const QString &color)
    { colorList << color; }

    void removeColor(const QString &color)
    { colorList.removeAll(color); }

    void removeColor(int index)
    { colorList.removeAt(index); }

    int colorCount() const
    { return colorList.count(); }

    QString getColor(int index) const
    { return colorList.value(index); }

private slots:
    void onCurrentIndexChanged(const QString &text);

private:
    QStringList colorList;
};

}

#endif // COLOR_LIST_ITEM_DELEGATE_H
