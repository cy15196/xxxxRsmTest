#ifndef FORMLOGIN_H
#define FORMLOGIN_H

#include <QDialog>

namespace Ui {
class FormLogin;
}

class FormLogin : public QDialog
{
    Q_OBJECT

public:
    explicit FormLogin(QWidget *parent = 0);
    ~FormLogin();

private:
    Ui::FormLogin *ui;
};

#endif // FORMLOGIN_H
