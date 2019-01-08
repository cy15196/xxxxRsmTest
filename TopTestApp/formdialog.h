#ifndef FORMDIALOG_H
#define FORMDIALOG_H

#include <QDialog>

namespace Ui {
class FormDialog;
}

class DialogBase;
class QAbstractButton;

class FormDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FormDialog(DialogBase*centralDialog, QWidget *parent = 0);
    ~FormDialog();

    ///设置显示哪些标准按钮
    void setStanderButtons(int buttons);

    DialogBase *getCentralDialog() const;
    void setCentralDialog(DialogBase *value);

//protected:
//    void mouseMoveEvent(QMouseEvent*event);
//    void mousePressEvent(QMouseEvent* event);



    void keyPressEvent(QKeyEvent *event);
protected:
    bool eventFilter(QObject *obj, QEvent *evt);
private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    DialogBase *centralDialog;
    Ui::FormDialog *ui;

    QPoint pos;
};

#endif // FORMDIALOG_H
