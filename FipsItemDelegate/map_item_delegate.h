/**
 * @file    map_item_delegate.h
 * @brief   item delegete for comboBox
 * @author  chl
 * @date    2017.6.19
 * @version 1.0.0
 * @license
 * @note    item delegete for comboBox
 *          data is a map
 */

#ifndef MAP_ITEM_DELEGATE_H
#define MAP_ITEM_DELEGATE_H

#include <QStyledItemDelegate>
#include <QMap>
#include <QString>
#include <QComboBox>
#include <QVariant>
#include <QApplication>
#include "fipsitemdelegate_global.h"

/**
 * @brief   namespace of company, Fips & zygk
 */
namespace Fips
{

/**
 * @class   MapItemDelegate
 * @brief   item delegete for comboBox
 * @author  chl
 * @date    2017.6.19
 * @note    data is a map
 *              key     text(QString)
 *              value   _valueType_(any type)
 *          create one and give it an option map, such as :
 *              Fips::MapItemDelegate *d = new Fips::MapItemDelegate(this);
 *              d->addOption("A", 11.11);
 *              d->addOption("B", 22.22);
 *              d->addOption("C", 33.33);
 *              d->addOption("D", 44.44);
 *              ui->tableWidget->setItemDelegateForColumn(0, d);
 */
template <typename _valueType_>
class FIPSITEMDELEGATESHARED_EXPORT MapItemDelegate : public QStyledItemDelegate
{
public:
    explicit MapItemDelegate(QObject *parent = FIPS_NULLPTR);
    ~MapItemDelegate();

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
     * @brief   options actions
     */
    void setOptions(QMap<QString, _valueType_> values);

    int addOption(const QString &text, const _valueType_ &value)
    {
        if (valueMap.keys().contains(text) || valueMap.values().contains(value))
            return 1;
        valueMap.insert(text, value);
        return 0;
    }

    void removeOption(const _valueType_ &value)
    { valueMap.remove(valueMap.key(value)); }

    void removeOption(const QString &text)
    { valueMap.remove(text); }

    QString getText(const _valueType_ &value) const
    { return valueMap.key(value); }

    _valueType_ getValue(const QString &text) const
    { return valueMap.value(text); }

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
    QMap<QString, _valueType_> valueMap;
    bool editable;
};

template<typename _valueType_>
MapItemDelegate<_valueType_>::MapItemDelegate(QObject *parent) :
    QStyledItemDelegate(parent),
    editable(false)
{

}

template<typename _valueType_>
MapItemDelegate<_valueType_>::~MapItemDelegate()
{

}

template<typename _valueType_>
void MapItemDelegate<_valueType_>::paint(QPainter *painter,
                                         const QStyleOptionViewItem &option,
                                         const QModelIndex &index) const
{
    QApplication::style()->drawItemText(painter,
                                        QRect(option.rect.x() + 4,
                                              option.rect.y(),
                                              option.rect.width(),
                                              option.rect.height()),
                                        Qt::AlignLeft | Qt::AlignVCenter,
                                        QPalette(QColor(255, 255, 255, 0)),
                                        true,
                                        valueMap.key(index.model()->data(index).value<_valueType_>()));
}

template<typename _valueType_>
QWidget *MapItemDelegate<_valueType_>::createEditor(QWidget *parent,
                                                    const QStyleOptionViewItem &/*option*/,
                                                    const QModelIndex &/*index*/) const
{
    QComboBox *comboBox=new QComboBox(parent);
    /// begin edit
    /// fixed order error when valueType is int or double
    /// origin code
       comboBox->addItems(valueMap.keys());
    /// edit code
    ///QMap<_valueType_,int> tempMap;
    ///foreach (_valueType_  value, valueMap.values()) {
    ///    tempMap.insert(value,0);
    ///}
    ///
    ///foreach (_valueType_  value, tempMap.keys()) {
    ///    comboBox->addItem(valueMap.key(value));
    ///}
    ///end edit

    comboBox->setEditable(editable);
    return comboBox;
}

template<typename _valueType_>
void MapItemDelegate<_valueType_>::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    dynamic_cast<QComboBox *>(editor)->setCurrentIndex(
                dynamic_cast<QComboBox *>(editor)->findText(
                    valueMap.key(index.data().value<_valueType_>())));
}

template<typename _valueType_>
void MapItemDelegate<_valueType_>::setModelData(QWidget *editor,
                                                QAbstractItemModel *model,
                                                const QModelIndex &index) const
{
    QString str = dynamic_cast<QComboBox *>(editor)->currentText();
    if (!valueMap.keys().contains(str))
        return;
    model->setData(index, QVariant::fromValue(valueMap.value(str)));
}

template<typename _valueType_>
void MapItemDelegate<_valueType_>::setOptions(QMap<QString, _valueType_> values)
{
    valueMap.clear();
#ifdef Q_COMPILER_RANGE_FOR
    for (QString text : values.keys())
#else
    QString text;
    foreach (text, values.keys())
#endif
        addOption(text, values.value(text));
}

}

#endif // MAP_ITEM_DELEGATE_H
