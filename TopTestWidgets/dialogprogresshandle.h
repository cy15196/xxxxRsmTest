#ifndef DIALOGPROGRESSHANDLE_H
#define DIALOGPROGRESSHANDLE_H

#include <QDialog>
#include "toptestwidgets_global.h"
namespace Ui {
class DialogProgressHandle;
}

class TOPTESTWIDGETSSHARED_EXPORT DialogProgressHandle : public QDialog
{
    Q_OBJECT

public:
    explicit DialogProgressHandle(QWidget *parent = 0);
    ~DialogProgressHandle();


private slots:
    void on_changedToType(int type);

    void onTableDataEdited(int type);

    void onDataSaved();

    void on_toolButton_Pic_clicked();

private:

    void initPortChoose();

    Ui::DialogProgressHandle *ui;
};

#endif // DIALOGPROGRESSHANDLE_H
