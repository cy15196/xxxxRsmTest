#include "dimension_item_delegate.h"
#include <QPainter>
#include <QApplication>

Fips::DimensionItemDelegate::DimensionItemDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
    dimentionList << "Kg" << "m" << "s" << "K" << "A" << "mol" << "cd";
}

Fips::DimensionItemDelegate::~DimensionItemDelegate()
{

}

void Fips::DimensionItemDelegate::paint(QPainter *painter,
                                        const QStyleOptionViewItem &option,
                                        const QModelIndex &index) const
{
    int x0 = option.rect.left() + 4;
    int y0 = option.rect.top();

    int fontSize = 10;
    int superscriptSize = 7;

    int nowRight = 0;
    int midWidth = 1;
    int superscriptUp = fontSize / 2;

    QFont tinyFont;
    QFont normalFont;
    tinyFont.setPointSize(superscriptSize);
    normalFont.setPointSize(fontSize);

    QString dimentionString = index.model()->data(index, Qt::DisplayRole).toString();
    dimentionString.remove("[").remove("]").remove("(").remove(")");
    QStringList valueList = dimentionString.split(QRegExp("[\\s]"));

    if (valueList.size() != 7)
    {
        QStyledItemDelegate::paint(painter, option, index);
        return;
    }

    ///begin to draw the edit
    for(int i=0; i<7; i++)
    {
        if (valueList[i].toInt() == 0)
            continue;
        painter->setFont(normalFont);

        QApplication::style()->drawItemText(painter,
                                            QRect(x0 + nowRight,
                                                  y0,
                                                  painter->fontMetrics().width("["),
                                                  option.rect.height()),
                                            Qt::AlignCenter,
                                            QPalette(),
                                            true,
                                            "[");
        nowRight += painter->fontMetrics().width("[") + midWidth;

        QApplication::style()->drawItemText(painter,
                                            QRect(x0 + nowRight,
                                                  y0,
                                                  painter->fontMetrics().width(dimentionList[i]),
                                                  option.rect.height()),
                                            Qt::AlignCenter,
                                            QPalette(),
                                            true,
                                            dimentionList[i]);
        nowRight += painter->fontMetrics().width(dimentionList[i]) + midWidth;

        if(valueList[i].toInt() != 1)
        {
            painter->setFont(tinyFont);

            QApplication::style()->drawItemText(painter,
                                                QRect(x0 + nowRight,
                                                      y0 - superscriptUp,
                                                      painter->fontMetrics().width(valueList[i]),
                                                      option.rect.height()),
                                                Qt::AlignCenter,
                                                QPalette(),
                                                true,
                                                valueList[i]);
            nowRight += painter->fontMetrics().width(valueList[i]) + midWidth;
        }

        painter->setFont(normalFont);

        QApplication::style()->drawItemText(painter,
                                            QRect(x0 + nowRight,
                                                  y0,
                                                  painter->fontMetrics().width("]"),
                                                  option.rect.height()),
                                            Qt::AlignCenter,
                                            QPalette(),
                                            true,
                                            "]");

        nowRight += painter->fontMetrics().width("]") + midWidth;
    }
}
