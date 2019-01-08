#ifndef FORMDATACURVE_H
#define FORMDATACURVE_H

#include <QWidget>

namespace Ui {
class FormDataCurve;
}

class FormDataCurve : public QWidget
{
    Q_OBJECT

public:
    explicit FormDataCurve(QWidget *parent = 0);
    ~FormDataCurve();

private:
    Ui::FormDataCurve *ui;
};

#endif // FORMDATACURVE_H
