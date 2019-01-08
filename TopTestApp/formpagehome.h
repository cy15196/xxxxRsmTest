#ifndef FORMPAGEHOME_H
#define FORMPAGEHOME_H

#include <QWidget>

namespace Ui {
class FormPageHome;
}

class FormPageHome : public QWidget
{
    Q_OBJECT

public:
    explicit FormPageHome(QWidget *parent = 0);
    ~FormPageHome();

signals:
    void pageTriggerButtonChecked(int pageEnum);

private slots:
    void onNeedToShowDialog(int pageEnum);

private:

    void initToolButtons();

    Ui::FormPageHome *ui;
};

#endif // FORMPAGEHOME_H
