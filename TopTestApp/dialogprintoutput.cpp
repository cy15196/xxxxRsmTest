#include "dialogprintoutput.h"
#include "ui_dialogprintoutput.h"

#include "clabso_ft_dpi.h"

DialogPrintOutput::DialogPrintOutput(QWidget *parent) :
    DialogBase(parent),
    ui(new Ui::DialogPrintOutput)
{
    ui->setupUi(this);
    ui->widget_printSelect->setEnabled(false);

    ui->radioButton_saveToPic->setVisible(false);
    ui->radioButton_printToFile->setVisible(false);

    this->setFixedSize(ClabsoFTDpi::convertSize(QSize(600,480)));
}

DialogPrintOutput::~DialogPrintOutput()
{
    delete ui;
}

void DialogPrintOutput::onButtonClicked(int role)
{

    int printContent = 0;
    if(ui->checkBox_info->isChecked())
    {
        printContent |= FileInfo;
    }
    else if(ui->checkBox_chart->isChecked())
    {
        printContent |= FileInfo;
    }
    else if(ui->checkBox_result->isChecked())
    {
        printContent |= FileInfo;
    }
    else if(ui->checkBox_device->isChecked())
    {
        printContent |= FileInfo;
    }




    switch (role) {
    case QDialogButtonBox::Ok:
    {
        int saveType = SaveToPDF;
        if(ui->radioButton_saveToPic->isChecked())
        {
            saveType = SaveToPic;
        }
        else if(ui->radioButton_saveToPDF->isChecked())
        {
            saveType = SaveToPDF;
        }
        else if(ui->radioButton_printToFile->isChecked())
        {
            saveType = PrintToFile;
        }

        emit this->needToPrintOutput(saveType,printContent);
    }
        break;
    default:
        break;
    }
}

void DialogPrintOutput::on_radioButton_printToFile_toggled(bool checked)
{
    ui->widget_printSelect->setEnabled(checked);
}
