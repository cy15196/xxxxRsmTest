#ifndef FORMNAVIBAR_H
#define FORMNAVIBAR_H

#include <QWidget>

namespace Ui {
class FormNaviBar;
}



class FormNaviBar : public QWidget
{
    Q_OBJECT

public:
    explicit FormNaviBar(QWidget *parent = 0);
    ~FormNaviBar();


signals:
    void pageTriggerButtonChecked(int pageEnum);

private slots:
    void on_toolButton_log_clicked();

    void onPageTriggerButtonChecked(int pageEnum);

    void onLanChooseChanged(int lanNo);

private:
    Ui::FormNaviBar *ui;

    void initNaviButtonGroup();
};

#endif // FORMNAVIBAR_H
