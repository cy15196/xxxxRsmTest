#include "line_color_item_delegate.h"
#include <QPainter>
#include <QColor>
#include <QDebug>
#include <QColorDialog>

#include <QPainterPath>

Fips::LineColorItemDelegate::LineColorItemDelegate(QObject *parent) :
    QStyledItemDelegate(parent),
    alpha(false)
{

}

Fips::LineColorItemDelegate::LineColorItemDelegate(bool hasAlpha, QObject *parent) :
    QStyledItemDelegate(parent),
    alpha(hasAlpha)
{

}

Fips::LineColorItemDelegate::~LineColorItemDelegate()
{

}

void Fips::LineColorItemDelegate::paint(QPainter *painter,
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

    QStyledItemDelegate::paint(painter,option,index);
    ///backgroud
    painter->fillRect(option.rect,QBrush(QColor(Qt::white), Qt::SolidPattern));

    ///line path
    QPainterPath path;

    int lineWidth = 2;
    QRect lineRect(option.rect.center().x() - 10 , option.rect.top() + 0.5 * option.rect.height() - 0.5 * lineWidth, 10 * 2, lineWidth );
    path.addRect(lineRect);

    QPainterPath cutRectPath;
    QRect cutRect(option.rect.center().x() - 4 , option.rect.top() + 0.5 * option.rect.height() - 0.5 * lineWidth, 4 * 2, lineWidth );
    cutRectPath.addRect(cutRect);

    QPainterPath circlePath;
    circlePath.addEllipse(option.rect.center(),6,6);

    QPainterPath cutPath;
    cutPath.addEllipse(option.rect.center(),4,4);

    circlePath -= cutPath;
    path -= cutRectPath;

    path += circlePath;


    path.toFillPolygon();

    painter->setRenderHint(QPainter::Antialiasing);
    painter->setBrush(QBrush(color,Qt::SolidPattern));
    painter->setPen(QPen(QColor(255, 255, 255), 1, Qt::SolidLine,
                        Qt::FlatCap, Qt::MiterJoin));
    painter->drawPath(path);


//    painter->setBrush(QBrush(QColor(Qt::white), Qt::SolidPattern));
//    painter->drawRect(option.rect);
//    painter->drawPath();
}

QWidget *Fips::LineColorItemDelegate::createEditor(QWidget *parent,
                                               const QStyleOptionViewItem &/*option*/,
                                               const QModelIndex &/*index*/) const
{
    QColorDialog *colorDialog = new QColorDialog(parent);
    if (alpha)
        colorDialog->setOption(QColorDialog::ShowAlphaChannel);
    else
        colorDialog->setOption(QColorDialog::ShowAlphaChannel, false);
    return colorDialog;
}

void Fips::LineColorItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
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

void Fips::LineColorItemDelegate::setModelData(QWidget *editor,
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
