#pragma execution_character_set("utf-8")

#include "formnavibar.h"
#include "ui_formnavibar.h"

#include <QDebug>
#include <QGroupBox>
#include <QMenu>

#include "rsmcoresignalenum.h"
#include "formlogin.h"

#include "rsmcoresignalandslotcontrolcenter.h"

#include "toptestsettingmanager.h"

#include <QListView>

inline void initPixmapLabel(QLabel* label,
                           const QString & iconPath,
                           double scaledRatio = 1.00 )
{
    //设置背景图片的来源
    label->setPixmap(QPixmap(iconPath).scaledToHeight(label->height()* scaledRatio));
}

inline void initToolButton(QToolButton* toolButton,
                           const QString & iconPath,
                           double scaledRatio = 1.00 )
{
    //设置背景图片的来源
    toolButton->setIcon(QIcon(iconPath));
    //设置填充按键这么大的
    toolButton->setIconSize(QSize(toolButton->width(),toolButton->height())*scaledRatio );
}


FormNaviBar::FormNaviBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormNaviBar)
{
    ui->setupUi(this);

    initNaviButtonGroup();

    this->setStyleSheet(this->styleSheet()+
                        "#widget_log{"
                        "width: 60px;"
                        "height: 60px;"
                        "border-style: solid;"
                        "border-width: 1px;"
                        "border-radius: 5px;"
                        "border-color: rgba(255,255,255,75);"
                        "}"
                        "#widget"
                        "{"
                        "background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, "
                        "stop:0 rgba(89, 132, 233, 255), "
                        "stop:0.495 rgba(89, 132, 233, 255), "
                        "stop:0.505 rgba(80, 125, 232, 255), "
                        "stop:1 rgba(80, 125, 232, 255));"
                        "}"
                        "QLabel#lable_logo{"
                        "background-color:rgba(89, 132, 233, 255); "
                        "}"
//                        "#comboBox_lan{"
//                        "background-color:rgba(200, 200, 233, 25);"
//                        "}"
//                        "QToolButton"
//                        "{"
//                        "border: 0px transparent;"
//                        "}"
//                        "QToolButton:hover:enabled"
//                        "{"
//                        "background-color:rgba(160, 180, 233, 80);"
//                        "}"
//                        "QToolButton:checked"
//                        "{"
//                        "background-color:rgba(160, 180, 233, 150);"
//                        "}"
//                        "QToolButton#Page_Home{"
//                        "border-style: solid;"
//                        "border-width: 1px;"
//                        "border-radius: 0px;"
//                        "border-color: rgba(255,255,255,75);"
//                        "}"
                        );

    initPixmapLabel(ui->label_logo,
                    ":/images/ic_logoshouye.png",
                    0.6);
}

FormNaviBar::~FormNaviBar()
{
    delete ui;
}

void FormNaviBar::initNaviButtonGroup()
{


   ui->comboBox_lan->setView(new QListView());

   ui->NaviButtonGroup->setTranslationContext(this->metaObject()->className());

   QWidget* groupBox =  ui->NaviButtonGroup->addGroup("NaviButtonGroup",false);
//   groupBox->

   QList<QToolButton*> toolButtonList;
   QToolButton* toolButton;


   toolButton =  ui->NaviButtonGroup->addToolButton(groupBox,QIcon(":/images/ic_shouye.png"),tr("Page_Home"),Page_Home,"Page_Home");
   toolButtonList << toolButton;

   toolButton =  ui->NaviButtonGroup->addToolButton(groupBox,QIcon(":/images/ic_quxianfenxi.png"),tr("Page_DataVisual"),Page_DataVisual,"Page_DataVisual");
   toolButtonList << toolButton;

   toolButton =  ui->NaviButtonGroup->addToolButton(groupBox,QIcon(":/images/ic_monifenxi.png"),tr("Page_VirtualReserach"),Page_VirtualReserach,"Page_VirtualReserach");
   toolButton->setEnabled(false);
   toolButtonList << toolButton;

   toolButton =  ui->NaviButtonGroup->addToolButton(groupBox,QIcon(":/images/ic_shishijiance.png"),tr("Page_RealtimeTest"),Page_RealtimeTest,"Page_RealtimeTest");

   toolButtonList << toolButton;

   toolButton =  ui->NaviButtonGroup->addToolButton(groupBox,QIcon(":/images/ic_wenjianshezhi.png"),tr("Page_FileSetting"),Page_FileSetting,"Page_FileSetting");
   toolButtonList << toolButton;

   toolButton =  ui->NaviButtonGroup->addToolButton(groupBox,QIcon(":/images/ic_zaixianjiankong.png"),tr("Page_OnlineMonitor"),Page_OnlineMonitor,"Page_OnlineMonitor");
   toolButton->setEnabled(false);
   toolButtonList << toolButton;

   toolButton =  ui->NaviButtonGroup->addToolButton(groupBox,QIcon(":/images/ic_spk.png"),tr("Page_SPKCPK"),Page_SPKCPK,"Page_SPKCPK");
   toolButton->setEnabled(false);
   toolButtonList << toolButton;

   toolButton =  ui->NaviButtonGroup->addToolButton(groupBox,QIcon(":/images/ic_bangzhu.png"),tr("Page_Help"),Page_Help,"Page_Help");
   toolButton->setEnabled(false);
   toolButtonList << toolButton;


   foreach (QToolButton* button, toolButtonList) {
       button->setCheckable(true);
       button->setAutoExclusive(true);
   }

   ui->NaviButtonGroup->setGroupToolButtonSize(groupBox,QSize(80,55),0.5);



   connect(ui->NaviButtonGroup,SIGNAL(toolButtonToggled(int)),
           this,SIGNAL(pageTriggerButtonChecked(int)));

   connect(this,SIGNAL(pageTriggerButtonChecked(int)),
           SLOT(onPageTriggerButtonChecked(int)));

   toolButtonList.first()->setChecked(true);
   onPageTriggerButtonChecked(Page_Home);

//   ui->toolButton_lan->setPopupMode(QToolButton::MenuButtonPopup);
//   QMenu* lanMenu = new QMenu(this);
//   ui->toolButton_lan->setDefaultAction(lanMenu->addAction(QIcon(":/ic_zh_CN"),tr("中文")));
//   lanMenu->addAction(QIcon(":/ic_en_US"),tr("English"));
//   ui->toolButton_lan->setMenu(lanMenu);
//   initToolButton(ui->toolButton_lan,":/images/ic_zhongwen.png");
//   ui->toolButton_lan->hide();

//   initPixmapLabel(ui->label_lan,":/ic_zh_CN");

   ui->toolButton_lan->hide();
   ui->label_lan->hide();


   ui->comboBox_lan->addItem(/*QIcon(":/ic_zh_CN"),*/tr("中文(简)"),"zh_CN");
   ui->comboBox_lan->addItem(/*QIcon(":/ic_zh_CN"),*/tr("中文(繁)"),"zh_TW");
   ui->comboBox_lan->addItem(/*QIcon(":/ic_en_US"),*/tr("English"),"en");
//   ui->comboBox_lan->setStyleSheet(ui->comboBox_lan->styleSheet()+
//                                   "QWidget{"
//                                   "background-color:rgba(200, 200, 233, 25);"
//                                   "}"
//                                   );

   connect(ui->comboBox_lan,SIGNAL(currentIndexChanged(int)),
           SLOT(onLanChooseChanged(int)));

   initPixmapLabel(ui->label_log,":/images/ic_dengluzhihui.png");

   ui->comboBox_lan->setEnabled(false);
//   ui->comboBox_lan->setVisible(false);
   ui->widget_log->setVisible(false);
//   ui->widget_lan->setVisible(false);
   ui->line_1->setVisible(false);
}

void FormNaviBar::on_toolButton_log_clicked()
{
    FormLogin login;
    emit RSMCoreSignalAndSlotControlCenter::instance()->startShowDialog();
    login.exec();
    emit RSMCoreSignalAndSlotControlCenter::instance()->endShowDialog();
}

void FormNaviBar::onPageTriggerButtonChecked(int pageEnum)
{
    QList<QToolButton*> buttonList =
            ui->NaviButtonGroup->getGroupButtonList("NaviButtonGroup");

    switch (pageEnum) {
    case Page_Home:
        {
            ui->NaviButtonGroup->setVisible(false);
        }
        break;
    default:
        {
            if(pageEnum<Page_Count)
            {
                ui->NaviButtonGroup->setVisible(true);
                buttonList[Page_Home]->setChecked(false);
                buttonList[pageEnum]->setChecked(true);
            }
        }
        break;
    }
}

void FormNaviBar::onLanChooseChanged(int lanNo)
{
    qDebug()<<"FormNaviBar::onLanChooseChanged";
    QString lanInfo = ui->comboBox_lan->currentData().toString();
    TopTestSettingManager::instance()->changeToLanguage(lanInfo);

//    initPixmapLabel(ui->label_lan,":/ic_"+lanInfo);
}
