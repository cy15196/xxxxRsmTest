#pragma execution_character_set("utf-8")

#include "RulerSliderGraphicItem.h"
#include "qpainter.h"
#include "qevent.h"
//#include "qtimer.h"
#include "qdebug.h"

#include <QStyleOptionGraphicsItem>
#include <QFontMetrics>

RulerSliderGraphicItem::RulerSliderGraphicItem(QGraphicsItem *parent) :	QGraphicsItem(parent)
{
    minValue = 0.0;
    maxValue = 100.0;
    value = 0.0;

    precision = 0;
    longStep = 10;
    shortStep = 1;
    space = 20;

    bgColorStart = QColor(100, 100, 100);
    bgColorEnd = QColor(60, 60, 60);
    lineColor = QColor(255, 255, 255);

    sliderColorTop = QColor(100, 184, 255);
    sliderColorBottom = QColor(235, 235, 235);

    tipBgColor = QColor(255, 255, 255);
    tipTextColor = QColor(50, 50, 50);

    pressed = false;
    currentValue = 0;
    sliderLastPot = QPointF(space, longLineHeight / 2);

    longLineHeight = 10;
    shortLineHeight = 5;
    sliderTopHeight = 5;
    sliderBottomHeight = 6;

//    setFont(QFont("Arial", 8));
}

void RulerSliderGraphicItem::resizeEvent(QResizeEvent *)
{
    longLineHeight = wholeSize.height() / 4;
    shortLineHeight = wholeSize.height() / 7;
    sliderTopHeight = wholeSize.height() / 7;
    sliderBottomHeight = wholeSize.height() / 6;

    if (this->isVisible()) {
        setValue(value);
    }
}

void RulerSliderGraphicItem::showEvent(QShowEvent *)
{
    resizeEvent(NULL);
}

//void RulerSliderGraphicItem::wheelEvent(QWheelEvent *e)
//{
//    //滚动的角度,*8就是鼠标滚动的距离
//    int degrees = e->delta() / 8;

//    //滚动的步数,*15就是鼠标滚动的角度
//    int steps = degrees / 15;

//    //如果是正数代表为左边移动,负数代表为右边移动
//    if (e->orientation() == Qt::Vertical) {
//        double value = currentValue - steps;

//        if (steps > 0) {
//            if (value > minValue) {
//                setValue(value);
//            } else {
//                setValue(minValue);
//            }
//        } else {
//            if (value < maxValue) {
//                setValue(value);
//            } else {
//                setValue(maxValue);
//            }
//        }
//    }
//}

//void RulerSliderGraphicItem::mousePressEvent(QMouseEvent *e)
//{
//    if (e->button() & Qt::LeftButton) {
//        if (sliderRect.contains(e->pos())) {
//            pressed = true;
//            update();
//        }
//    }
//}

//void RulerSliderGraphicItem::mouseReleaseEvent(QMouseEvent *e)
//{
//    pressed = false;
//    update();
//}

//void RulerSliderGraphicItem::mouseMoveEvent(QMouseEvent *e)
//{
//    if (pressed) {
//        if (e->pos().x() >= lineLeftPot.x() && e->pos().x() <= lineRightPot.x()) {
//            double totalLineWidth = lineRightPot.x() - lineLeftPot.x();
//            double dx = e->pos().x() - lineLeftPot.x();
//            double ratio = (double)dx / totalLineWidth;
//            sliderLastPot = QPointF(e->pos().x(), sliderTopPot.y());

//            currentValue = (maxValue - minValue) * ratio + minValue;
//            this->value = currentValue;
////            emit valueChanged(currentValue);
//            update();
//        }
//    }
//}

void RulerSliderGraphicItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget)
    static bool inPaint = false;
    if(inPaint)
        return;
//    qDebug()<<"RulerSliderGraphicItem::paint";
    //绘制准备工作,启用反锯齿
    inPaint = true;
    painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

//    qDebug()<<"RulerSliderGraphicItem::paint"<<option->exposedRect<<widget;

    //绘制背景
    drawBg(painter,option);
    //绘制标尺
    drawRule(painter,option);
    //绘制滑块
    drawSlider(painter,option);
    //绘制当前值的提示
    drawTip(painter,option);

    inPaint = false;
}

//void RulerSliderGraphicItem::paintEvent(QPaintEvent *)
//{
//    //绘制准备工作,启用反锯齿
//    QPainter painter(this);
//    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

//    //绘制背景
//    drawBg(&painter);
//    //绘制标尺
//    drawRule(&painter);
//    //绘制滑块
//    drawSlider(&painter);
//    //绘制当前值的提示
//    drawTip(&painter);
//}

void RulerSliderGraphicItem::drawBg(QPainter *painter, const QStyleOptionGraphicsItem *option)
{
    painter->save();
    painter->setPen(Qt::NoPen);
    QLinearGradient bgGradient(0, 0, 0, wholeSize.height() - option->fontMetrics.height());
    bgGradient.setColorAt(0.0, bgColorStart);
    bgGradient.setColorAt(1.0, bgColorEnd);
    painter->setBrush(bgGradient);
    painter->drawRect(space, space, wholeSize.width() - 2 * space , wholeSize.height() - option->fontMetrics.height() - space);
    painter->restore();

    //绘制外框
    painter->save();
    painter->setPen(lineColor);
    painter->drawRect(space, space, wholeSize.width() - 2 * space , wholeSize.height() - option->fontMetrics.height() - space);
    painter->restore();
}

void RulerSliderGraphicItem::drawRule(QPainter *painter, const QStyleOptionGraphicsItem *option)
{
    painter->save();
    painter->setPen(lineColor);

    //绘制横向标尺底部线,居中
    double initX = space;
    double initY = (double)wholeSize.height() - option->fontMetrics.height() ;
    lineLeftPot = QPointF(initX, initY);
    lineRightPot = QPointF(wholeSize.width() - initX, initY);
    painter->drawLine(lineLeftPot, lineRightPot);

    //绘制纵向标尺刻度
    double length = wholeSize.width() - 2 * space;
    //计算每一格移动多少
    double increment = length / (maxValue - minValue);

    //根据范围值绘制刻度值及刻度值
    for (int i = minValue; i <= maxValue; i = i + shortStep) {
        if (i % longStep == 0) {
            QPointF topPot(initX, initY - longLineHeight);
            QPointF bottomPot(initX, initY);
            painter->drawLine(topPot, bottomPot);

            QString strValue = QString("%1").arg((double)i, 0, 'f', precision);
            double textWidth = option->fontMetrics.width(strValue);
            double textHeight = option->fontMetrics.height();
            QPointF textPot(initX - textWidth / 2, initY + textHeight);
            painter->drawText(textPot, strValue);
        } else {
            QPointF topPot(initX, initY - shortLineHeight);
            QPointF bottomPot(initX, initY);
            painter->drawLine(topPot, bottomPot);
        }

        initX += increment * shortStep;
    }

    painter->restore();
}

void RulerSliderGraphicItem::drawSlider(QPainter *painter, const QStyleOptionGraphicsItem *option)
{
    painter->save();

    setValue(this->value);

    //绘制滑块上部分三角形
    sliderTopPot = QPointF(sliderLastPot.x(), lineLeftPot.y() - longLineHeight / 4);
    sliderLeftPot = QPointF(sliderTopPot.x() - wholeSize.width() / 100, sliderTopPot.y() - sliderTopHeight);
    sliderRightPot = QPointF(sliderTopPot.x() + wholeSize.width() / 100, sliderTopPot.y() - sliderTopHeight);

//    sliderTopPot = QPointF(sliderLastPot.x(), lineLeftPot.y() - longLineHeight / 4);
//    sliderLeftPot = QPointF(sliderTopPot.x() - wholeSize.width() / 100, sliderTopPot.y() + sliderTopHeight);
//    sliderRightPot = QPointF(sliderTopPot.x() + wholeSize.width() / 100, sliderTopPot.y() + sliderTopHeight);

//    QPixmap locationPixmap = QPixmap(":/images/ic_weizhidingwei.png").scaledToHeight(wholeSize.height() - option->fontMetrics.height() - space * 0.75 );

//    painter->drawPixmap( QPointF(sliderTopPot.x() - locationPixmap.width() * 0.5 - 0.3025 , space * 0.75 ),locationPixmap);


    painter->setPen(sliderColorTop);
    painter->setBrush(sliderColorTop);

    QVector<QPointF> potVec;
    potVec.append(sliderTopPot);
    potVec.append(sliderLeftPot);
    potVec.append(sliderRightPot);
    painter->drawPolygon(potVec);

    //绘制滑块下部分矩形
    double indicatorLength = sliderRightPot.x() - sliderLeftPot.x();

    QPointF handleBottomRightPot(sliderLeftPot.x(), sliderLeftPot.y() - sliderBottomHeight * 2);
    sliderRect = QRectF(handleBottomRightPot, sliderRightPot);

//    QPointF handleBottomRightPot(sliderLeftPot.x() + indicatorLength, sliderLeftPot.y() + sliderBottomHeight);
//    sliderRect = QRectF(sliderLeftPot, handleBottomRightPot);

    QString strValue = QString("%1").arg(value, 0, 'f', precision);
    double textLength = option->fontMetrics.width(strValue);
    double textHeight = option->fontMetrics.height();

    QPointF tipRectTopLeftPot(sliderRect.topLeft().x() - textLength - 10, sliderRect.topLeft().y());

    QPointF tipRectBottomRightPot(tipRectTopLeftPot.x() + textLength + 10, tipRectTopLeftPot.y() + textHeight + 5);
    tipRect = QRectF(tipRectTopLeftPot, tipRectBottomRightPot);

    painter->setPen(sliderColorBottom);
    painter->setBrush(sliderColorBottom);
    painter->drawRect(sliderRect);
    painter->restore();
}

void RulerSliderGraphicItem::drawTip(QPainter *painter, const QStyleOptionGraphicsItem *option)
{
//    if (!pressed) {
//        return;
//    }

    painter->save();
    painter->setPen(tipTextColor);
    painter->setBrush(tipBgColor);
    painter->drawRect(tipRect);
    QString strValue = QString("%1").arg(value, 0, 'f', precision);
    painter->drawText(tipRect, Qt::AlignCenter, strValue);
    painter->restore();
}

QSizeF RulerSliderGraphicItem::getWholeSize() const
{
    return wholeSize;
}

void RulerSliderGraphicItem::setWholeSize(const QSizeF &value)
{
    wholeSize = value;
}

double RulerSliderGraphicItem::getMinValue() const
{
    return this->minValue;
}

double RulerSliderGraphicItem::getMaxValue() const
{
    return this->maxValue;
}

double RulerSliderGraphicItem::getValue() const
{
    return this->value;
}

int RulerSliderGraphicItem::getPrecision() const
{
    return this->precision;
}

int RulerSliderGraphicItem::getLongStep() const
{
    return this->longStep;
}

int RulerSliderGraphicItem::getShortStep() const
{
    return this->shortStep;
}

int RulerSliderGraphicItem::getSpace() const
{
    return this->space;
}

QColor RulerSliderGraphicItem::getBgColorStart() const
{
    return this->bgColorStart;
}

QColor RulerSliderGraphicItem::getBgColorEnd() const
{
    return this->bgColorEnd;
}

QColor RulerSliderGraphicItem::getLineColor() const
{
    return this->lineColor;
}

QColor RulerSliderGraphicItem::getSliderColorTop() const
{
    return this->sliderColorTop;
}

QColor RulerSliderGraphicItem::getSliderColorBottom() const
{
    return this->sliderColorBottom;
}

QColor RulerSliderGraphicItem::getTipBgColor() const
{
    return this->tipBgColor;
}

QColor RulerSliderGraphicItem::getTipTextColor() const
{
    return this->tipTextColor;
}

QSize RulerSliderGraphicItem::sizeHint() const
{
    return QSize(500, 70);
}

QSize RulerSliderGraphicItem::minimumSizeHint() const
{
    return QSize(50, 50);
}

void RulerSliderGraphicItem::setRange(double minValue, double maxValue)
{
    //如果最小值大于或者等于最大值则不设置
    if (minValue >= maxValue) {
        return;
    }

    this->minValue = minValue;
    this->maxValue = maxValue;
    setValue(minValue);
}

void RulerSliderGraphicItem::setRange(int minValue, int maxValue)
{
    setRange((double)minValue, (double)maxValue);
}

void RulerSliderGraphicItem::setMinValue(double minValue)
{
    setRange(minValue, maxValue);
}

void RulerSliderGraphicItem::setMaxValue(double maxValue)
{
    setRange(minValue, maxValue);
}

void RulerSliderGraphicItem::setValue(double value)
{
    //值小于最小值或者值大于最大值则无需处理
    if (value < minValue || value > maxValue) {
        return;
    }

    double lineWidth = wholeSize.width() - 2 * space;
    double ratio = (double)(value - minValue) / (maxValue - minValue);
    double x = lineWidth * ratio;
    double newX = x + space;
    double y = space + longLineHeight - 10;
    sliderLastPot = QPointF(newX, y);

    this->value = value;
    this->currentValue = value;
//    emit valueChanged(value);
    update();
}

void RulerSliderGraphicItem::setValue(int value)
{
    setValue((double)value);
}

void RulerSliderGraphicItem::setPrecision(int precision)
{
    //最大精确度为 3
    if (precision <= 3 && this->precision != precision) {
        this->precision = precision;
        update();
    }
}

void RulerSliderGraphicItem::setLongStep(int longStep)
{
    //短步长不能超过长步长
    if (longStep < shortStep) {
        return;
    }

    if (this->longStep != longStep) {
        this->longStep = longStep;
        update();
    }
}

void RulerSliderGraphicItem::setShortStep(int shortStep)
{
    //短步长不能超过长步长
    if (longStep < shortStep) {
        return;
    }

    if (this->shortStep != shortStep) {
        this->shortStep = shortStep;
        update();
    }
}

void RulerSliderGraphicItem::setSpace(int space)
{
    if (this->space != space) {
        this->space = space;
        update();
    }
}

void RulerSliderGraphicItem::setBgColorStart(const QColor &bgColorStart)
{
    if (this->bgColorStart != bgColorStart) {
        this->bgColorStart = bgColorStart;
        update();
    }
}

void RulerSliderGraphicItem::setBgColorEnd(const QColor &bgColorEnd)
{
    if (this->bgColorEnd != bgColorEnd) {
        this->bgColorEnd = bgColorEnd;
        update();
    }
}

void RulerSliderGraphicItem::setLineColor(const QColor &lineColor)
{
    if (this->lineColor != lineColor) {
        this->lineColor = lineColor;
        update();
    }
}

void RulerSliderGraphicItem::setSliderColorTop(const QColor &sliderColorTop)
{
    if (this->sliderColorTop != sliderColorTop) {
        this->sliderColorTop = sliderColorTop;
        update();
    }
}

void RulerSliderGraphicItem::setSliderColorBottom(const QColor &sliderColorBottom)
{
    if (this->sliderColorBottom != sliderColorBottom) {
        this->sliderColorBottom = sliderColorBottom;
        update();
    }
}

void RulerSliderGraphicItem::setTipBgColor(const QColor &tipBgColor)
{
    if (this->tipBgColor != tipBgColor) {
        this->tipBgColor = tipBgColor;
        update();
    }
}

void RulerSliderGraphicItem::setTipTextColor(const QColor &tipTextColor)
{
    if (this->tipTextColor != tipTextColor) {
        this->tipTextColor = tipTextColor;
        update();
    }
}
