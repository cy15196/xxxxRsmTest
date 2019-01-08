#include "formdraglabel.h"

#include <QMouseEvent>

FormDragLabel::FormDragLabel(QWidget *parent)
    :QToolButton(parent)
{
    this->setFixedSize(20,20);
}

void FormDragLabel::setPortNo(const int &portNo)
{
    this->setText(QString::number(portNo));
}

void FormDragLabel::setPortColor(const QString &colorName)
{
    QColor color(colorName);
    QColor oppColor;
    oppColor.setRed(255 - color.red());
    oppColor.setGreen(255 - color.green());
    oppColor.setBlue(255 - color.blue());
    this->setStyleSheet("QToolButton"
                        "{"
                        "background-color:"+color.name()+";"
                        "color:"+oppColor.name()+";"
                        "}");
}

void FormDragLabel::mouseMoveEvent(QMouseEvent *event)
{
    QPoint nowPos = this->mapToParent(event->pos());
    this->move(this->geometry().topLeft() + nowPos - pos);
    pos = nowPos;
    emit this->dragPosChanged();
    QToolButton::mouseMoveEvent(event);
}

void FormDragLabel::mousePressEvent(QMouseEvent *event)
{
    pos = this->mapToParent(event->pos());
    QToolButton::mousePressEvent(event);
}

void FormDragLabel::setPos(const QPoint &value)
{
    pos = value;
    move(pos);
}

QPoint FormDragLabel::getPos() const
{
    return pos;
}
