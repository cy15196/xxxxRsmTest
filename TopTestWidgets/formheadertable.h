#ifndef FORMHEADERTABLE_H
#define FORMHEADERTABLE_H

#include <QWidget>
#include "toptestwidgets_global.h"

namespace Ui {
class FormHeaderTable;
}

class TOPTESTWIDGETSSHARED_EXPORT FormHeaderTable : public QWidget
{
    Q_OBJECT

public:
    explicit FormHeaderTable(QWidget *parent = 0);
    ~FormHeaderTable();

public slots:
    void changeToType(int type);

signals:
    void changedToType(int type);

protected:
    void changeEvent(QEvent*event);

private:
    Ui::FormHeaderTable *ui;
};

#endif // FORMHEADERTABLE_H
