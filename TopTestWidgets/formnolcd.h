#ifndef FORMNOLCD_H
#define FORMNOLCD_H

#include <QWidget>

#include "toptestwidgets_global.h"

namespace Ui {
class FormNoLCD;
}

class TOPTESTWIDGETSSHARED_EXPORT FormNoLCD : public QWidget
{
    Q_OBJECT

public:
    explicit FormNoLCD(QWidget *parent = 0);
    ~FormNoLCD();

    void setNo(int no);
    int getNo() const;

    void setLCDVisiable(bool show);

public slots:
    void setLCDValue(double value);

private:

    int no;

    Ui::FormNoLCD *ui;
};

#endif // FORMNOLCD_H
