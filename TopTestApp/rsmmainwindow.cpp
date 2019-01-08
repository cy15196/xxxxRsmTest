#include "rsmmainwindow.h"
#include "ui_rsmmainwindow.h"

#include "formpagehome.h"
#include "formpagedatavisual.h"
#include "formnavibar.h"
#include "formfilesetting.h"
#include "formpagerealtimemonitor.h"

#include <QLabel>
#include <QStackedLayout>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QDialog>
#include <QTranslator>
#include <QApplication>

#include "rsmcoresignalandslotcontrolcenter.h"

#include "toptestsettingmanager.h"

RSMMainWindow::RSMMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RSMMainWindow)
{
//    QTranslator* lan = new QTranslator;
//    lan->load("Clabso_zh_CN.qm");
//    QApplication::instance()->installTranslator(lan);

    showMockCount = 0;

    ui->setupUi(this);

    QStackedLayout* layout = new QStackedLayout;
    layout->setStackingMode(QStackedLayout::StackAll);
    ui->centralwidget->setLayout(layout);

    mockWidget = new QWidget(this);
    mockWidget->setObjectName("mockWidget");
    layout->addWidget(mockWidget);
    mockWidget->setStyleSheet(mockWidget->styleSheet()+
                              "QWidget#mockWidget"
                              "{"
                              "background-color:rgba(0, 0, 0,127);"
                              "}");
    mockWidget->setVisible(false);

    QWidget* mainWidget = new QWidget(this);
    mainWidget->setLayout(new QVBoxLayout);
    FormNaviBar* naviBar = new FormNaviBar(this);
    mainWidget->layout()->addWidget(naviBar);
    QStackedWidget* mainStackWidget = new QStackedWidget(this);
    mainWidget->layout()->addWidget(mainStackWidget);
    layout->addWidget(mainWidget);

    mainWidget->layout()->setMargin(0);
    mainWidget->layout()->setSpacing(0);

    FormPageHome * homePage = new FormPageHome(this);
    FormPageDataVisual * dataPage = new FormPageDataVisual(this) ;
    FormPageRealtimeMonitor* monitorPage =  new FormPageRealtimeMonitor(this);

    mainStackWidget->addWidget(homePage);
    mainStackWidget->addWidget(dataPage);
    mainStackWidget->addWidget(new QLabel("2",this));
    mainStackWidget->addWidget(monitorPage);
    mainStackWidget->addWidget(new FormFileSetting(this));

    mainStackWidget->addWidget(new QLabel("5",this));
    mainStackWidget->addWidget(new QLabel("6",this));
    mainStackWidget->addWidget(new QLabel("7",this));


    connect(naviBar,SIGNAL(pageTriggerButtonChecked(int)),
            mainStackWidget,SLOT(setCurrentIndex(int)));

    connect(homePage,SIGNAL(pageTriggerButtonChecked(int)),
            mainStackWidget,SLOT(setCurrentIndex(int)));

    connect(homePage,SIGNAL(pageTriggerButtonChecked(int)),
            naviBar,SLOT(onPageTriggerButtonChecked(int)));

    connect(monitorPage,SIGNAL(monitorOnChanged(bool)),
            dataPage,SLOT(onMonitorSwitchChanged(bool)));

    connect(RSMCoreSignalAndSlotControlCenter::instance(),SIGNAL(startShowDialog()),
            this,SLOT(onStartShowDialog()));
    connect(RSMCoreSignalAndSlotControlCenter::instance(),SIGNAL(endShowDialog()),
            this,SLOT(onEndShowDialog()));

    loadSystemSettings();

}

RSMMainWindow::~RSMMainWindow()
{
    delete ui;
}

void RSMMainWindow::loadSystemSettings()
{
    ///load language
    TopTestSettingManager::instance()->changeToLanguage("zh_CN");

    ///load unit type
    TopTestSettingManager::instance()->loadFromSystemSettings();

}

void RSMMainWindow::onStartShowDialog()
{
    showMockCount++;
    mockWidget->setVisible(showMockCount);
}

void RSMMainWindow::onEndShowDialog()
{
    showMockCount--;
    if(showMockCount<0)
        showMockCount = 0;
    mockWidget->setVisible(showMockCount);
}

void RSMMainWindow::closeEvent(QCloseEvent *event)
{
    ///before close app ,do some save things
}
