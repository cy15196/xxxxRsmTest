#include "clabsotoolbutton.h"
#include <QPainter>
#include <QStyle>
#include <QCommonStyle>
#include <QProxyStyle>
#include <QStyleOption>
#include <QStyleFactory>
#include <QStyleOptionToolButton>
#include <QDebug>

ClabsoToolButton::ClabsoToolButton(QWidget *parent) :
    QToolButton(parent)
{

}

ClabsoToolButton::~ClabsoToolButton()
{

}

QSize ClabsoToolButton::sizeHint() const
{
    QSize size = QToolButton::sizeHint();
    QStyleOptionToolButton opt;
    initStyleOption(&opt);
    if (toolButtonStyle() == Qt::ToolButtonTextUnderIcon)
    {
        if (popupMode() == MenuButtonPopup)
            size.setWidth(size.width() - style()->pixelMetric(QStyle::PM_MenuButtonIndicator, &opt, this));
        size.setHeight(size.height() + style()->pixelMetric(QStyle::PM_MenuButtonIndicator, &opt, this));
    }
    else if (popupMode() == InstantPopup && menu())
    {
        size.setWidth(size.width() + style()->pixelMetric(QStyle::PM_MenuButtonIndicator, &opt, this));
    }
    return size;
}
