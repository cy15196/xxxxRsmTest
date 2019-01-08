#pragma execution_character_set("utf-8")

#include "dialogsystemsetting.h"
#include "ui_dialogsystemsetting.h"


#include "toptestsettingmanager.h"
#include "toptestcore.h"


#include <QLabel>
#include <QRadioButton>
#include <QSpinBox>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>
#include <QDir>




DialogSystemSetting::DialogSystemSetting(QWidget *parent) :
    DialogBase(parent),
    ui(new Ui::DialogSystemSetting)
{
    ui->setupUi(this);
    init();
}

DialogSystemSetting::~DialogSystemSetting()
{
    delete ui;
}

void DialogSystemSetting::onButtonClicked(int role)
{
    switch (role) {
    case QDialogButtonBox::Ok:
    case QDialogButtonBox::Apply:
    {
        foreach (QString key, labelToButtonListMap.keys()) {
            int num = -1;
            foreach (QRadioButton*button, labelToButtonListMap.value(key)) {
                if(button->isChecked())
                {
                    num = labelToButtonListMap.value(key).indexOf(button);
                    break;
                }
            }
            if(num >=0)
            {
                TopTestSettingManager::instance()->setSystemSettingKeyValue(key,num);
                if(key == "TemperatureUnit:")
                {
                    TopTestCore::instance()->setUnitType(
                               (TopTest::UnitType) (TopTestCore::instance()->getUnitType() | num));
                }
            }
        }


        foreach (QString key, labelToEditorMap.keys()) {
            int value = labelToEditorMap.value(key)->value();
            TopTestSettingManager::instance()->setSystemSettingKeyValue(key,value);
        }

        foreach (QString key, labelToLineEditorMap.keys()) {
            QString value = labelToLineEditorMap.value(key)->text();
            TopTestSettingManager::instance()->setSystemSettingKeyValue(key,value);
        }

        TopTestSettingManager::instance()->saveToSystemSettings();
    }
        break;
    default:
        break;
    }
}

int DialogSystemSetting::standerButtons()
{
    return DialogBase::standerButtons() | QDialogButtonBox::Apply;
}

void DialogSystemSetting::browsePath()
{
    QString currentPath = labelToLineEditorMap["WorkDir:"]->text();
    QString path = QFileDialog::getExistingDirectory(this,tr("Choose path as work dir"),currentPath);
    QDir dir(path);
    if(dir.exists())
    {
        labelToLineEditorMap["WorkDir:"]->setText(dir.absolutePath());
    }
}

void DialogSystemSetting::loadSystemSetting()
{
    TopTestSettingManager::instance()->loadFromSystemSettings();
    foreach (QString key, labelToButtonListMap.keys()) {
        bool ok;
        int num = TopTestSettingManager::instance()->getSystemSettingKeyValue(key).toInt(&ok);
        if(ok && num>=0 && num < labelToButtonListMap.value(key).count())
            labelToButtonListMap.value(key).at(num)->setChecked(true);
    }

    foreach (QString key, labelToEditorMap.keys()) {
        bool ok;
        int num = TopTestSettingManager::instance()->getSystemSettingKeyValue(key).toInt(&ok);
        if(ok && num > 0)
            labelToEditorMap.value(key)->setValue(num);
    }

    foreach (QString key, labelToLineEditorMap.keys()) {
        QString value = TopTestSettingManager::instance()->getSystemSettingKeyValue(key).toString();
        labelToLineEditorMap.value(key)->setText(value);
    }
}


void DialogSystemSetting::init()
{
    ui->listWidget->setMinimumSize(QSize(600,400));
    {
        QWidget* widget = new QWidget(ui->listWidget);
        QHBoxLayout* hLayout = new QHBoxLayout;
        widget->setLayout(hLayout);
        QListWidgetItem * item = new QListWidgetItem(ui->listWidget);
        item->setSizeHint(QSize(200,60));
        ui->listWidget->setItemWidget(item,widget);
        ui->listWidget->addItem(item);
        QString key = ("GlobalPortNum:");
        hLayout->addWidget(new QLabel(tr("GlobalPortNum:")));
        hLayout->addSpacing(40);

        QSpinBox* portSetBox = new QSpinBox;
        portSetBox->setMinimum(1);
        portSetBox->setMaximum(16);
        portSetBox->setValue(16);
        hLayout->addWidget(portSetBox );

        labelToEditorMap[key] = portSetBox;

//        labelToButtonListMap[key] = buttonList;

        hLayout->addSpacerItem(new QSpacerItem(40,20,QSizePolicy::Expanding));
    }

    {
        QWidget* widget = new QWidget(ui->listWidget);
        QHBoxLayout* hLayout = new QHBoxLayout;
        widget->setLayout(hLayout);
        QListWidgetItem * item = new QListWidgetItem(ui->listWidget);
        item->setSizeHint(QSize(200,60));
        ui->listWidget->setItemWidget(item,widget);
        ui->listWidget->addItem(item);
        QString key = ("WorkDir:");
        hLayout->addWidget(new QLabel(tr("WorkDir:")));
        hLayout->addSpacing(40);

        QLineEdit* pathEdit = new QLineEdit;
        pathEdit->setPlaceholderText(tr("Select path as the root of data file saved"));
        pathEdit->setEnabled(false);
//        pathEdit->setText("D:/Rsm");
        hLayout->addWidget(pathEdit);
        labelToLineEditorMap[key] = pathEdit;

        QPushButton* pushButton = new QPushButton;
        connect(pushButton,SIGNAL(clicked(bool)),
                this,SLOT(browsePath()));
        pushButton->setText(tr("Browse"));
        hLayout->addWidget(pushButton);



//        labelToButtonListMap[key] = buttonList;

        hLayout->addSpacerItem(new QSpacerItem(40,20,QSizePolicy::Expanding));
    }

    {
        QWidget* widget = new QWidget(ui->listWidget);
        QHBoxLayout* hLayout = new QHBoxLayout;
        widget->setLayout(hLayout);
        QListWidgetItem * item = new QListWidgetItem(ui->listWidget);
        item->setSizeHint(QSize(200,60));
        ui->listWidget->setItemWidget(item,widget);
        ui->listWidget->addItem(item);
        QString key = ("TemperatureUnit:");
        hLayout->addWidget(new QLabel(tr("TemperatureUnit:")));
        hLayout->addSpacing(40);
        QStringList namelist;
        namelist<<("C");
        namelist<<("F");

        QList<QRadioButton*> buttonList;
        foreach (QString name, namelist) {
            buttonList<<new QRadioButton(name);
        }

        foreach (QRadioButton*button, buttonList) {
            hLayout->addWidget(button);
        }
        buttonList.first()->setChecked(true);
        labelToButtonListMap[key] = buttonList;

        hLayout->addSpacerItem(new QSpacerItem(40,20,QSizePolicy::Expanding));
    }

    {
        QWidget* widget = new QWidget(ui->listWidget);
        QHBoxLayout* hLayout = new QHBoxLayout;
        widget->setLayout(hLayout);
        QListWidgetItem * item = new QListWidgetItem(ui->listWidget);
        item->setSizeHint(QSize(200,60));
        ui->listWidget->setItemWidget(item,widget);
        ui->listWidget->addItem(item);
        QString key = ("Language:");
        hLayout->addWidget(new QLabel(tr("Language:")));
        hLayout->addSpacing(40);

        QStringList namelist;
        namelist<<tr("Chinese(S)");
        namelist<<tr("Chinese(T)");
        namelist<<tr("English");

        QList<QRadioButton*> buttonList;
        foreach (QString name, namelist) {
            buttonList<<new QRadioButton(name);
        }

        foreach (QRadioButton*button, buttonList) {
            hLayout->addWidget(button);
        }
        buttonList.first()->setChecked(true);
        labelToButtonListMap[key] = buttonList;

        hLayout->addSpacerItem(new QSpacerItem(40,20,QSizePolicy::Expanding));
    }

//    {
//        QWidget* widget = new QWidget(ui->listWidget);
//        QHBoxLayout* hLayout = new QHBoxLayout;
//        widget->setLayout(hLayout);
//        QListWidgetItem * item = new QListWidgetItem(ui->listWidget);
//        item->setSizeHint(QSize(200,60));
//        ui->listWidget->setItemWidget(item,widget);
//        ui->listWidget->addItem(item);
//        QString key = ("TimeUnit:");
//        hLayout->addWidget(new QLabel(tr("TimeUnit:")));
//        hLayout->addSpacing(40);

//        QStringList namelist;
//        namelist<<("Hour");
//        namelist<<("Minute");
//        namelist<<("Second");

//        QList<QRadioButton*> buttonList;
//        foreach (QString name, namelist) {
//            buttonList<<new QRadioButton(name);
//        }

//        foreach (QRadioButton*button, buttonList) {
//            hLayout->addWidget(button);
//        }
//        buttonList.first()->setChecked(true);
//        labelToButtonListMap[key] = buttonList;

//        hLayout->addSpacerItem(new QSpacerItem(40,20,QSizePolicy::Expanding));
//    }

//    {
//        QWidget* widget = new QWidget(ui->listWidget);
//        QHBoxLayout* hLayout = new QHBoxLayout;
//        widget->setLayout(hLayout);
//        QListWidgetItem * item = new QListWidgetItem(ui->listWidget);
//        item->setSizeHint(QSize(200,60));
//        ui->listWidget->setItemWidget(item,widget);
//        ui->listWidget->addItem(item);
//        QString key = ("DeviceType:");
//        hLayout->addWidget(new QLabel(tr("DeviceType:")));
//        hLayout->addSpacing(40);

//        QStringList namelist;
//        namelist<<("Rsm");
//        namelist<<("Rsm Wireless");
//        namelist<<("TopTest");
//        namelist<<("Rsm Monitor");

//        QList<QRadioButton*> buttonList;
//        foreach (QString name, namelist) {
//            buttonList<<new QRadioButton(name);
//        }

//        foreach (QRadioButton*button, buttonList) {
//            hLayout->addWidget(button);
//        }
//        buttonList.first()->setChecked(true);
//        labelToButtonListMap[key] = buttonList;

//        hLayout->addSpacerItem(new QSpacerItem(40,20,QSizePolicy::Expanding));
//    }

    loadSystemSetting();

}

