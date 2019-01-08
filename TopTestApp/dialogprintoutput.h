#ifndef DIALOGPRINTOUTPUT_H
#define DIALOGPRINTOUTPUT_H

#include <QWidget>
#include "dialogbase.h"

namespace Ui {
class DialogPrintOutput;
}

class DialogPrintOutput : public DialogBase
{
    Q_OBJECT


public:
    explicit DialogPrintOutput(QWidget *parent = 0);
    ~DialogPrintOutput();

    enum PrintContent{
        FileInfo = 0x01,
        Chart = 0x02,
        ProgressResult = 0x04,
        DeviceInfo = 0x08
    };

    enum DialogPrintType{
        SaveToPic,
        SaveToPDF,
        PrintToFile
    };

    void onButtonClicked(int role);
//    int standerButtons();

signals:
    void needToPrintOutput(int saveType,int printContent);

private slots:
    void on_radioButton_printToFile_toggled(bool checked);

private:
    Ui::DialogPrintOutput *ui;
};

#endif // DIALOGPRINTOUTPUT_H
