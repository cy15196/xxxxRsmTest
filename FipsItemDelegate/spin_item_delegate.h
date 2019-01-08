/**
 * @file    spin_item_delegate.h
 * @brief   item delegete for spinBox
 * @author  chl
 * @date    2017.6.19
 * @version 1.0.0
 * @license
 * @note
 */

#ifndef SPIN_ITEM_DELEGATE_H
#define SPIN_ITEM_DELEGATE_H

#include <QStyledItemDelegate>
#include <QString>
#include "fipsitemdelegate_global.h"

/**
 * @brief   namespace of company, Fips & zygk
 */
namespace Fips
{

/**
 * @class   SpinItemDelegate
 * @brief   item delegete for spinBox
 * @author  chl
 * @date    2017.6.19
 * @note    create one and give a option map, such as :
 *              Fips::SpinItemDelegate *d = new Fips::SpinItemDelegate(this);
 *              d->setSpinBox(-1, 100, 1, "L", "mm", "auto");
 *              ui->tableWidget->setItemDelegateForColumn(0, d);
 */
class FIPSITEMDELEGATESHARED_EXPORT SpinItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit SpinItemDelegate(QObject *parent = FIPS_NULLPTR);
    ~SpinItemDelegate();

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

    void setPrefix(const QString &prefix)
    { this->prefix = prefix; }

    const QString &getPrefix() const
    { return prefix; }

    void setSuffix(const QString &suffix)
    { this->suffix = suffix; }

    const QString &getSuffix() const
    { return suffix; }

    void setDecimals(int decimals)
    { this->decimals = decimals; }

    int getDecimals() const
    { return decimals; }

    void setMinimum(double minimum)
    { this->minimum = minimum; }

    double getMinimum() const
    { return minimum; }

    void setMaximum(double maximum)
    { this->maximum = maximum; }

    double getMaximum() const
    { return maximum; }

    void setSingalStep(double singalStep)
    { this->singalStep = singalStep; }

    double getSingalStep() const
    { return singalStep; }

    void setSpecialValueText(const QString &specialValueText)
    { this->specialValueText = specialValueText; }

    const QString &getSpecialValueText() const
    { return specialValueText; }

    void setSpinBox(double minimum,
                    double maximum,
                    double singalStep = 1.00,
                    int decimals = 2,
                    const QString &prefix = "",
                    const QString &suffix = "",
                    const QString &specialValueText = "")
    {
        this->prefix = prefix;
        this->suffix = suffix;
        this->decimals = decimals;
        this->minimum = minimum;
        this->maximum = maximum;
        this->singalStep = singalStep;
        this->specialValueText = specialValueText;
    }

    void setSpinBox(int minimum,
                    int maximum,
                    int singalStep = 1,
                    const QString &prefix = "",
                    const QString &suffix = "",
                    const QString &specialValueText = "")
    {
        this->prefix = prefix;
        this->suffix = suffix;
        this->decimals = 0;
        this->minimum = static_cast<double>(minimum);
        this->maximum = static_cast<double>(maximum);
        this->singalStep = static_cast<double>(singalStep);
        this->specialValueText = specialValueText;
    }

private:
    /**
     * @brief   string front data
     */
    QString prefix;

    /**
     * @brief   string behind data
     */
    QString suffix;

    /**
     * @brief   The number of digits after the decimal point.
     */
    int decimals;

    /**
     * @brief   minimum value
     */
    double minimum;

    /**
     * @brief   maximum value
     */
    double maximum;

    /**
     * @brief   every step increase or decrease
     */
    double singalStep;

    /**
     * @brief   special text for the minimum value
     * @date    While it is the minimum value, data is not a int or double. It is astring.
     */
    QString specialValueText;
};

}

#endif // SPIN_ITEM_DELEGATE_H
