#ifndef FORMSTATETIME_H
#define FORMSTATETIME_H

#include <QWidget>

namespace Ui {
class FormStateTime;
}

class FormStateTime : public QWidget
{
    Q_OBJECT

public:
    explicit FormStateTime(QWidget *parent = 0);
    ~FormStateTime();

private:

    void reset();

    Ui::FormStateTime *ui;
};

#endif // FORMSTATETIME_H
