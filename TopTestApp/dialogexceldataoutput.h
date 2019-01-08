#ifndef DIALOGEXCELDATAOUTPUT_H
#define DIALOGEXCELDATAOUTPUT_H

#include <QWidget>
#include "dialogbase.h"


namespace Ui {
class DialogExcelDataOutput;
}

class DialogExcelDataOutput : public DialogBase
{
    Q_OBJECT

public:
    explicit DialogExcelDataOutput(QWidget *parent = 0);
    ~DialogExcelDataOutput();

    int standerButtons();

    void onButtonClicked(int role);

    QString getOutputStr() const;
    void setOutputStr(const QString &value);

private:

    QString outputStr;

    Ui::DialogExcelDataOutput *ui;
};

#endif // DIALOGEXCELDATAOUTPUT_H
