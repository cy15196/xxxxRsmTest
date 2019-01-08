#ifndef DIALOGSYSTEMSETTING_H
#define DIALOGSYSTEMSETTING_H

#include <QWidget>
#include <QMap>
#include "dialogbase.h"

namespace Ui {
class DialogSystemSetting;
}

class QRadioButton;
class QSpinBox;
class QLineEdit;

class DialogSystemSetting : public DialogBase
{
    Q_OBJECT

public:
    explicit DialogSystemSetting(QWidget *parent = 0);
    ~DialogSystemSetting();

    void onButtonClicked(int role);
    int standerButtons();

private slots:
    void browsePath();
    void loadSystemSetting();

private:

    void init();

    QMap<QString,QList<QRadioButton*>> labelToButtonListMap;
    QMap<QString,QSpinBox*> labelToEditorMap;
    QMap<QString,QLineEdit*> labelToLineEditorMap;

    Ui::DialogSystemSetting *ui;
};

#endif // DIALOGSYSTEMSETTING_H
