#ifndef FORMDRAGLABEL_H
#define FORMDRAGLABEL_H

#include <QToolButton>

class FormDragLabel: public QToolButton
{
    Q_OBJECT
public:
    FormDragLabel(QWidget* parent);

    QPoint getPos() const;
    void setPos(const QPoint &value);

public slots:
    void setPortNo(const int& portNo);
    void setPortColor(const QString& colorName);

signals:
    void dragPosChanged();

protected:
    void mouseMoveEvent(QMouseEvent*event);
    void mousePressEvent(QMouseEvent* event);

private:
    QPoint pos;
};

#endif // FORMDRAGLABEL_H
