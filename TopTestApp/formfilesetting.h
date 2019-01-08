#ifndef FORMFILESETTING_H
#define FORMFILESETTING_H

#include <QWidget>

namespace Ui {
class FormFileSetting;
}

class FormFileSetting : public QWidget
{
    Q_OBJECT

public:
    explicit FormFileSetting(QWidget *parent = 0);
    ~FormFileSetting();

protected:
    void changeEvent(QEvent* event);

private slots:
    void onChangePage(int dialogEnum);

    void onReloadSystemSetting();

private:

    void init();
    Ui::FormFileSetting *ui;
};

#endif // FORMFILESETTING_H
