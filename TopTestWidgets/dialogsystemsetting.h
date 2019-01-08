#ifndef DIALOGSYSTEMSETTING_H
#define DIALOGSYSTEMSETTING_H

#include <QWidget>

#include "toptestwidgets_global.h"

namespace Ui {
class DialogSystemSetting;
}

class TOPTESTWIDGETSSHARED_EXPORT DialogSystemSetting : public QWidget
{
    Q_OBJECT

public:
    explicit DialogSystemSetting(QWidget *parent = 0);
    ~DialogSystemSetting();

private slots:
    void on_toolButton_TopTest_clicked();

    void on_toolButton_RSMII_clicked();

    void on_toolButton_OK_clicked();


    void on_comboBox_Port_currentTextChanged(const QString &arg1);

signals:

    void languageChanged(const QString& lan);

private:

    void init();

    Ui::DialogSystemSetting *ui;
    QString lan;
};

#endif // DIALOGSYSTEMSETTING_H
