#include "spin_item_delegate.h"
#include <QApplication>
#include <QDoubleSpinBox>

Fips::SpinItemDelegate::SpinItemDelegate(QObject *parent) :
    QStyledItemDelegate(parent),
    prefix(""),
    suffix(""),
    decimals(0),
    minimum(0.00),
    maximum(100.00),
    singalStep(1.00),
    specialValueText("")
{

}

Fips::SpinItemDelegate::~SpinItemDelegate()
{

}

void Fips::SpinItemDelegate::paint(QPainter *painter,
                                   const QStyleOptionViewItem &option,
                                   const QModelIndex &index) const
{
    QString str;
    if (!prefix.isEmpty())
        str += prefix + " ";
    str += index.data().toString();
    if (!suffix.isEmpty())
        str += " " + suffix;

    if (!specialValueText.isEmpty() && index.data().toString() == specialValueText)
        str = specialValueText;

    QApplication::style()->drawItemText(painter,
                                        QRect(option.rect.x() + 4,
                                              option.rect.y(),
                                              option.rect.width(),
                                              option.rect.height()),
                                        Qt::AlignLeft | Qt::AlignVCenter,
                                        QPalette(QColor(255, 255, 255, 0)),
                                        true,
                                        str);
}

QWidget *Fips::SpinItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &/*option*/, const QModelIndex &/*index*/) const
{
    QDoubleSpinBox *spinBox = new QDoubleSpinBox(parent);
    if (!prefix.isEmpty())
        spinBox->setPrefix(prefix + " ");
    if (!suffix.isEmpty())
        spinBox->setSuffix(" " + suffix);
    spinBox->setMinimum(minimum);
    spinBox->setMaximum(maximum);
    spinBox->setSingleStep(singalStep);
    spinBox->setDecimals(decimals);
    if (!specialValueText.isEmpty())
        spinBox->setSpecialValueText(specialValueText);
    return spinBox;
}

void Fips::SpinItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString str = index.data().toString();
    double d = str.toDouble();
    if (!specialValueText.isEmpty() && str == specialValueText)
        d = minimum;
    dynamic_cast<QDoubleSpinBox *>(editor)->setValue(d);
}

void Fips::SpinItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    model->setData(index, dynamic_cast<QDoubleSpinBox *>(editor)->cleanText());
}
