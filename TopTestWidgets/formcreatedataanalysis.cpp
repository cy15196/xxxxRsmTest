#include "formcreatedataanalysis.h"
#include "ui_formcreatedataanalysis.h"
#include "topteststructdata.h"

#include "toptestcore.h"

FormCreateDataAnalysis::FormCreateDataAnalysis(
        const QMap<QString, QVariant> &dataMap,
        QWidget *parent) :
    ProjectSetting(dataMap,parent),
    ui(new Ui::FormCreateDataAnalysis)
{
    ui->setupUi(this);
    this->setWindowTitle("DataAnalysis");

    ui->comboBox_Device->addItems(TopTestCore::instance()->getDefinedDeviceList());
    ui->comboBox_Process->addItems(TopTestCore::instance()->getDefinedProcessList());

    ui->textBrowser->append(tr("Create a DataAnalysis file\n"
                               "You can choose the some default setting here:\n"
                               "  1.Device   the device used to get data\n"
                               "  2.Process  processes used to analysis"));
}

void FormCreateDataAnalysis::init(const QMap<QString, QVariant> &dataMap)
{

}

QMap<QString,QVariant> FormCreateDataAnalysis::getDataMap()
{
    QMap<QString,QVariant> a;

    a.insert("Device",ui->comboBox_Device->currentText());
    a.insert("Process",ui->comboBox_Process->currentText());
    return a;
}

FormCreateDataAnalysis::~FormCreateDataAnalysis()
{
    delete ui;
}
