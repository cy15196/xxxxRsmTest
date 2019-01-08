#include "dialogfilesettingchoose.h"
#include "ui_dialogfilesettingchoose.h"

#include <QSettings>
#include <QCoreApplication>
#include <QDebug>

DialogFileSettingChoose::DialogFileSettingChoose(QWidget *parent) :
    DialogBase(parent),
    ui(new Ui::DialogFileSettingChoose)
{
    ui->setupUi(this);
    typeToComboBoxEditMap["Device"] = ui->comboBox_Device;
    typeToComboBoxEditMap["Product"] = ui->comboBox_Product;
    typeToComboBoxEditMap["Progress"] = ui->comboBox_Progress;
    init();
}

DialogFileSettingChoose::~DialogFileSettingChoose()
{
    delete ui;
}

void DialogFileSettingChoose::onButtonClicked(int role)
{
    switch (role) {
    case QDialogButtonBox::Ok:
    case QDialogButtonBox::Apply:
    {
        if(ui->label_Product->font().bold())
        {
            emit productChooseChanged(ui->comboBox_Product->currentText());
        }
        if(ui->label_Device->font().bold())
        {
            emit deviceChooseChanged(ui->comboBox_Device->currentText());
        }
        if(ui->label_Progress->font().bold())
        {
            emit progressChooseChanged(ui->comboBox_Progress->currentText());
        }
        if(ui->label_velocity->font().bold())
        {
            emit velocityChanged(ui->doubleSpinBox_velocity->value());
        }

        QFont font;
        ui->label_Product->setFont(font);
        ui->label_Device->setFont(font);
        ui->label_Progress->setFont(font);
        ui->label_velocity->setFont(font);

//        ui->label_Product->

    }
        break;
    default:
        break;
    }
}


void DialogFileSettingChoose::init()
{

    ui->pushButton_device->setVisible(false);
    ui->pushButton_product->setVisible(false);
    ui->pushButton_progress->setVisible(false);

    ui->doubleSpinBox_velocity->setMaximum(1e3);
    ui->doubleSpinBox_velocity->setMinimum(-1e3);

    QSettings appSetting(QCoreApplication::organizationName(),QCoreApplication::applicationName());
    appSetting.beginGroup("FileSettings");
    foreach (QString fileType, appSetting.childGroups()) {
        qDebug()<<"fileType:"<<fileType;
        appSetting.beginGroup(fileType);
        QComboBox* typeBox = typeToComboBoxEditMap[fileType];
        if(typeBox)
        {
            typeBox->addItems(appSetting.childKeys());
//            QString firstSettingFile = appSetting.childKeys().first();
        }
        appSetting.endGroup();
    }
    appSetting.endGroup();

    QFont font;
    ui->label_Product->setFont(font);
    ui->label_Device->setFont(font);
    ui->label_Progress->setFont(font);
    ui->label_velocity->setFont(font);

//    ui->label_Product->setText(ui->label_Product->text().remove("*"));
//    ui->label_Device->setText(ui->label_Device->text().remove("*"));
//    ui->label_Progress->setText(ui->label_Progress->text().remove("*"));
}

void DialogFileSettingChoose::on_comboBox_Product_currentIndexChanged(int index)
{
    QFont font;
    font.setBold(true);
    ui->label_Product->setFont(font);
}

void DialogFileSettingChoose::on_comboBox_Device_currentIndexChanged(int index)
{
    QFont font;
    font.setBold(true);
    ui->label_Device->setFont(font);
}

void DialogFileSettingChoose::on_comboBox_Progress_currentIndexChanged(int index)
{
    QFont font;
    font.setBold(true);
    ui->label_Progress->setFont(font);
}

double DialogFileSettingChoose::getVelocity() const
{
    return velocity;
}

void DialogFileSettingChoose::setVelocity(double value)
{
    velocity = value;
    ui->doubleSpinBox_velocity->setValue(velocity);
    QFont font;
    ui->label_velocity->setFont(font);
}

void DialogFileSettingChoose::setCurrentProcess(const QString &processName)
{
    QString buildInStr = ("BuiltIn:")+processName;
    ui->comboBox_Progress->addItem(buildInStr);
    ui->comboBox_Progress->setCurrentText(buildInStr);
    QFont font;
    ui->label_Progress->setFont(font);
}

void DialogFileSettingChoose::setCurrentDevice(const QString &deviceName)
{
    QString buildInStr = ("BuiltIn:")+deviceName;
    ui->comboBox_Device->addItem(buildInStr);
    ui->comboBox_Device->setCurrentText(buildInStr);
    QFont font;
    ui->label_Device->setFont(font);
}

void DialogFileSettingChoose::setCurrentProduct(const QString &productName)
{
    QString buildInStr = ("BuiltIn:")+productName;
    ui->comboBox_Product->addItem(buildInStr);
    ui->comboBox_Product->setCurrentText(buildInStr);
    QFont font;
    ui->label_Product->setFont(font);
}

int DialogFileSettingChoose::standerButtons()
{
    return DialogBase::standerButtons() | QDialogButtonBox::Apply;
}

void DialogFileSettingChoose::on_doubleSpinBox_velocity_valueChanged(double arg1)
{
    QFont font;
    font.setBold(true);
    ui->label_velocity->setFont(font);
}
