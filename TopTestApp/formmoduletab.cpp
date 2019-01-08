#include "formmoduletab.h"
#include "ui_formmoduletab.h"

#include "dialogsystemsetting.h"
#include "formdatavisual.h"
#include "dialogdeviceandprogresshanding.h"
#include "formwelcome.h"
#include "formdevicewidget.h"
#include "formreport.h"

#include <QDebug>

#include "clabso_ft_io.h"

FormModuleTab::FormModuleTab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormModuleTab)
{
    ui->setupUi(this);

    init();

    ui->toolButton_Setting->setVisible(false);
}

FormModuleTab::~FormModuleTab()
{
    delete ui;
}

QList<QWidget *> FormModuleTab::getWidgetList()
{
    return widgetMap.values();
}

void FormModuleTab::onToolButtonToggled(bool checked)
{
    if(!checked)
        return;
    QToolButton* button = qobject_cast<QToolButton*> (sender());
    if(!button)
        return;


    QWidget* w = widgetMap[button];
    emit this->changedToWidget(w);
    CsLogs("Change to %2(%1)",w->metaObject()->className(),button->text());
}

void FormModuleTab::onFileOpened()
{
    ui->toolButton_Data->setChecked(true);
}

void FormModuleTab::onShowWidget()
{
    QWidget* w = qobject_cast<QWidget* > (sender());
    qDebug()<<w;
    if(w)
    {
        widgetMap.key(w)->setChecked(true);
    }
}

void FormModuleTab::init()
{

    FormWelcome* welcome = new FormWelcome(this);
    connect(this,SIGNAL(refreshWelcomeWidget()),
            welcome,SLOT(refreshHistoryList()));
    connect(welcome,SIGNAL(needToOpenFile(QString)),
            this,SIGNAL(needToOpenFile(QString)));

    FormDataVisual* data = new FormDataVisual(this);
    FormReport* report = new FormReport(this);

    connect(data,SIGNAL(gotoReport()),
            report,SIGNAL(needToShow()));

    connect(report,SIGNAL(needToShow()),
            this,SLOT(onShowWidget()));

    widgetMap.insert(ui->toolButton_Welcome,welcome);
    widgetMap.insert(ui->toolButton_Setting,new DialogSystemSetting(this));
    widgetMap.insert(ui->toolButton_Progress,new DialogDeviceAndProgressHanding(this));

    widgetMap.insert(ui->toolButton_Data,data);
    widgetMap.insert(ui->toolButton_Device,new FormDeviceWidget(this));
    widgetMap.insert(ui->toolButton_Report,report);

    foreach (QToolButton* button, widgetMap.keys()) {
        connect(button, SIGNAL(toggled(bool)),this,SLOT(onToolButtonToggled(bool)));
    }
}
