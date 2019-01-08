#include "color_item_delegate.h"
#include <QPainter>
#include <QColor>
#include <QDebug>
#include <QColorDialog>

Fips::ColorItemDelegate::ColorItemDelegate(QObject *parent) :
    QStyledItemDelegate(parent),
    alpha(false)
{

}

Fips::ColorItemDelegate::ColorItemDelegate(bool hasAlpha, QObject *parent) :
    QStyledItemDelegate(parent),
    alpha(hasAlpha)
{

}

Fips::ColorItemDelegate::~ColorItemDelegate()
{

}

void Fips::ColorItemDelegate::paint(QPainter *painter,
                                    const QStyleOptionViewItem &option,
                                    const QModelIndex &index) const
{
#if QT_VERSION >= 0x050200
    QString colorStr = index.data().toString();
    QColor color(colorStr);
#else
    QString colorStr = index.data().toString();
    QColor color;
    if (colorStr.count() == 9)
    {
        QString alpha16 = colorStr.left(3).right(2);
        colorStr.remove(1, 2);
        color = QColor(colorStr);
        bool ok;
        color.setAlpha(alpha16.toInt(&ok, 16));
    }
    else
    {
        color = QColor(colorStr);
    }
#endif
    if(color.isValid())
    {
        painter->drawTiledPixmap(option.rect, QPixmap(":/img_vtk/colorBack"));
        painter->setBrush(QBrush(color, Qt::SolidPattern));
        painter->drawRect(option.rect);
    }
    else
        QStyledItemDelegate::paint(painter,option,index);

}

QWidget *Fips::ColorItemDelegate::createEditor(QWidget *parent,
                                               const QStyleOptionViewItem &option,
                                               const QModelIndex &index) const
{
    QString colorStr = index.data().toString();
    QColor color(colorStr);

    if(color.isValid())
    {
        QColorDialog *colorDialog = new QColorDialog(parent);
        if (alpha)
            colorDialog->setOption(QColorDialog::ShowAlphaChannel);
        else
            colorDialog->setOption(QColorDialog::ShowAlphaChannel, false);
        return colorDialog;
    }
    else
        return QStyledItemDelegate::createEditor(parent,option,index);

}

void Fips::ColorItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
#if QT_VERSION >= 0x050200
    QString colorStr = index.data().toString();
    QColor color(colorStr);
#else
    QString colorStr = index.data().toString();
    QColor color;
    if (colorStr.count() == 9)
    {
        QString alpha16 = colorStr.left(3).right(2);
        colorStr.remove(1, 2);
        color = QColor(colorStr);
        bool ok;
        color.setAlpha(alpha16.toInt(&ok, 16));
    }
    else
    {
        color = QColor(colorStr);
    }
#endif
    dynamic_cast<QColorDialog *>(editor)->setCurrentColor(color);
}

void Fips::ColorItemDelegate::setModelData(QWidget *editor,
                                           QAbstractItemModel *model,
                                           const QModelIndex &index) const
{
    QColor c = dynamic_cast<QColorDialog *>(editor)->currentColor();
    QString colorStr;
    if (alpha)
    {
#if QT_VERSION >= 0x050200
        colorStr = c.name(QColor::HexArgb);
#else
        QString alpha16 = QString().number(c.alpha(), 16);
        if (alpha16.count() > 2)
            alpha16 = alpha16.right(2);
        else if (alpha16.count() == 1)
            alpha16 = "0" + alpha16;
        else if (alpha16.count() == 0)
            alpha16 = "00";
        colorStr = c.name();
        colorStr.insert(1, alpha16);
#endif
    }
    else
        colorStr = c.name();
    model->setData(index, colorStr);
}
