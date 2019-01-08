#include "formpagehome.h"
#include "ui_formpagehome.h"


#include <QToolButton>
#include <QDebug>

#include "rsmcoresignalenum.h"
#include "dialogharddevicesetting.h"
#include "dialogsystemsetting.h"
#include "dialogcurrentlinkdevices.h"
#include "rsmcoresignalandslotcontrolcenter.h"

#include "formdialog.h"

#include "rsmusb.h"

#include "formlinksetting.h"

FormPageHome::FormPageHome(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormPageHome)
{
    ui->setupUi(this);

    initToolButtons();

    ui->widget_info->setStyleSheet(ui->widget_info->styleSheet()+
                                  "QWidget#widget_info{"
                                  "border-style: solid;"
                                  "border-width: 1px 0px 0px 0px;"
                                  "border-color: #cfd7e3;"
                                  "background-color:rgba(245,248,251,127);"
                                  "}");

    this->setStyleSheet(this->styleSheet()+
                        "QWidget#widget{"
                        "border-image: url(:/images/bg_beijingshouye.png); "
                        "}"
//                        "QToolButton"
//                        "{"
//                        "border-style: transparent;"
//                        "border-width: 5px;"
//                        "border-radius: 10px;"
//                        "border-color: rgba(255,255,255,75);"
//                        "background-color:rgba(255, 255, 255, 255)"
//                        "}"
//                        "QToolButton:hover:enabled"
//                        "{"
//                        "background-color:rgba(160, 180, 233, 127)"
//                        "}"
//                        "QToolButton:checked"
//                        "{"
//                        "background-color:rgba(160, 180, 233, 255)"
//                        "}"
//                        "QToolButton#page_home{"
//                        "border-style: solid;"
//                        "border-width: 1px;"
//                        "border-radius: 5px;"
//                        "border-color: rgba(255,255,255,75);"
//                        "}"
                        );
}

FormPageHome::~FormPageHome()
{
    delete ui;
}

void FormPageHome::onNeedToShowDialog(int pageEnum)
{

    qDebug()<<"FormPageHome::onNeedToShowDialog"<<pageEnum;

    if(pageEnum < Dialog_OnlineTestSetting || pageEnum > Dialog_CurrentLinkDevice)
    {
        qDebug()<<"It's not home page dialog";
        return;
    }

    emit RSMCoreSignalAndSlotControlCenter::instance()->startShowDialog();


    switch (pageEnum) {
    case Dialog_HardSetting:
    {
        FormDialog formDialog(new DialogHardDeviceSetting,this);
        formDialog.setWindowTitle(tr("Dialog_HardSetting"));
        formDialog.exec();
    }
        break;
    case Dialog_SystemSetting:
    {
        FormDialog formDialog(new DialogSystemSetting,this);
        formDialog.setWindowTitle(tr("Dialog_SystemSetting"));
        formDialog.exec();
    }
        break;
    case Dialog_OnlineTestSetting:
    {
        FormDialog formDialog(new DialogHardDeviceSetting,this);
        formDialog.setWindowTitle(tr("Dialog_OnlineTestSetting"));
        formDialog.exec();
    }
        break;
    case Dialog_CurrentLinkDevice:
    {
        FormDialog formDialog(new DialogCurrentLinkDevices,this);
        formDialog.setWindowTitle(tr("Dialog_CurrentLinkDevice"));
        formDialog.exec();
    }
        break;
    default:
        break;
    }


    emit RSMCoreSignalAndSlotControlCenter::instance()->endShowDialog();

}

void FormPageHome::initToolButtons()
{

    ui->MainWay->setTranslationContext(this->metaObject()->className());


    QWidget* widget = ui->MainWay->addGroup("MainButtonGroup",false);
    widget->layout()->setSpacing(30);

    QList<QToolButton*> buttonList;
    QToolButton* toolButton;

    buttonList <<ui->MainWay->addToolButton(widget,QIcon("://images/ic_home_quxianfenxi.png"),
                                            tr("Home_DataVisual"),Home_DataVisual,"Home_DataVisual");

    buttonList <<ui->MainWay->addToolButton(widget,QIcon("://images/ic_home_yingjianshezhi.png"),
                                            tr("Dialog_HardSetting"),Dialog_HardSetting,"Dialog_HardSetting");

    toolButton = ui->MainWay->addToolButton(widget,QIcon("://images/ic_home_xitongshezhi.png"),
                                            tr("Dialog_SystemSetting"),Dialog_SystemSetting,"Dialog_SystemSetting");
    toolButton->setEnabled(true);
    buttonList <<toolButton;

    toolButton = ui->MainWay->addToolButton(widget,QIcon("://images/ic_home_zaixianjiankongshezhi.png"),
                                            tr("Dialog_OnlineTestSetting"),Dialog_OnlineTestSetting,"Dialog_OnlineTestSetting");
    toolButton->setEnabled(false);
    buttonList <<toolButton;

    toolButton = ui->MainWay->addToolButton(widget,QIcon("://images/ic_home_dangqianlianjieliebiao.png"),
                                            tr("Dialog_CurrentLinkDevice"),Dialog_CurrentLinkDevice,"Dialog_CurrentLinkDevice");
    toolButton->setEnabled(false);
    buttonList <<toolButton;


    foreach (QToolButton* button, buttonList) {
        button->setCheckable(true);
        button->setAutoExclusive(true);
//        button->setStyleSheet();
    }

    ui->MainWay->setGroupToolButtonSize(widget,QSize(120,200),0.8);


    connect(ui->MainWay,SIGNAL(toolButtonToggled(int)),
            this,SIGNAL(pageTriggerButtonChecked(int)));

    connect(this,SIGNAL(pageTriggerButtonChecked(int)),
            SLOT(onNeedToShowDialog(int)));
}
