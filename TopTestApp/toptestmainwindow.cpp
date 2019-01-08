#include "toptestmainwindow.h"
#include "ui_toptestmainwindow.h"

#include <QHBoxLayout>

#include "toptestcore.h"
#include "formdevicewidget.h"
#include "toptestenum.h"
#include "chartwidget.h"
#include "dialogdevicehandle.h"
#include "dialogprogresshandle.h"

#include "toptestusb.h"
#include "dialogsystemsetting.h"
#include "formdatavisual.h"
#include "dialogdeviceandprogresshanding.h"

#include "clabsotoolbutton.h"
#include "toptestactions.h"

#include "clabso_ft_dpi.h"

TopTestMainWindow::TopTestMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TopTestMainWindow)
{
    ui->setupUi(this);
    initToolBar();

    foreach (QWidget* w, ui->widget->getWidgetList()) {
        ui->stackedWidget->addWidget(w);
    }

    connect(ui->widget,SIGNAL(changedToWidget(QWidget*)),
            ui->stackedWidget,SLOT(setCurrentWidget(QWidget*)));

    this->setMinimumSize(ClabsoFTDpi::convertSize(1000),ClabsoFTDpi::convertSize(600));

    onProjectOpened("");
}

TopTestMainWindow::~TopTestMainWindow()
{
    delete ui;
    delete TopTestUsb::instance();
}

void TopTestMainWindow::onProjectOpened(const QString &filePath)
{
    this->setWindowTitle(qApp->applicationName()
                         +"-"+qApp->applicationVersion()
                         +"    "+filePath);
}

void TopTestMainWindow::initToolBar()
{
    QSize bigButtonIconSize = QSize(ClabsoFTDpi::convertSize(28),ClabsoFTDpi::convertSize(28));
    constexpr int maxRow = 3;
    int column;

    ui->toolBar->setMinimumHeight(ClabsoFTDpi::convertSize(100));
    ui->toolBar->setMaximumHeight(ClabsoFTDpi::convertSize(100));
    QHBoxLayout* toolBarLayout = new QHBoxLayout(ui->toolBar);
    toolBarLayout->setMargin(0);
    ui->toolBar->setLayout(toolBarLayout);

/// begin
    QGroupBox *groupBox_begin = new QGroupBox(ui->toolBar);
    groupBox_begin->setTitle(tr("Tools"));
    groupBox_begin->setObjectName("Tools");
    toolBarLayout->addWidget(groupBox_begin);

    QGridLayout *gridLayout_begin = new QGridLayout(groupBox_begin);
    column = 0;

    /// save
//    ClabsoToolButton *toolButton_save = new ClabsoToolButton(groupBox_begin);
//    gridLayout_begin->addWidget(toolButton_save, 0, column++, maxRow, 1);
//    toolButton_save->setPopupMode(QToolButton::MenuButtonPopup);
//    toolButton_save->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
//    toolButton_save->setAutoRaise(true);
//    toolButton_save->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
//    toolButton_save->setIconSize(bigButtonIconSize);
//    toolButton_save->setFocusPolicy(Qt::NoFocus);

//    QMenu *menu_save = new QMenu(groupBox_begin);
//    toolButton_save->setMenu(menu_save);
//    QAction* saveAction = new SaveProjectAction(QIcon(":/img/save"), tr("Save"),"Save");
//    toolButton_save->setDefaultAction(saveAction);
//    menu_save->addAction(saveAction);

    ClabsoToolButton *toolButton_open = new ClabsoToolButton(groupBox_begin);
    gridLayout_begin->addWidget(toolButton_open, 0, column++, maxRow, 1);
    toolButton_open->setPopupMode(QToolButton::DelayedPopup);
    toolButton_open->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    toolButton_open->setAutoRaise(true);
    toolButton_open->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    toolButton_open->setIconSize(bigButtonIconSize);
    toolButton_open->setFocusPolicy(Qt::NoFocus);

    OpenAction*openaction = new OpenAction(QIcon(":/img/open"),tr("Open"),"Open",groupBox_begin);
    toolButton_open->setDefaultAction(openaction);
    connect(openaction,SIGNAL(fileOpened(QString)),
            this,SLOT(onProjectOpened(QString)));

    ClabsoToolButton *toolButton_new = new ClabsoToolButton(groupBox_begin);
    gridLayout_begin->addWidget(toolButton_new, 0, column++, maxRow, 1);
    toolButton_new->setPopupMode(QToolButton::DelayedPopup);
    toolButton_new->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    toolButton_new->setAutoRaise(true);
    toolButton_new->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    toolButton_new->setIconSize(bigButtonIconSize);
    toolButton_new->setFocusPolicy(Qt::NoFocus);

    NewAction*newaction = new NewAction(QIcon(":/img/new"),tr("New"),"New",groupBox_begin);
//    newaction->setObjectName("New");
//    newaction->setIcon(QIcon(":/img/new"));
    toolButton_new->setDefaultAction(newaction);

    connect(newaction,SIGNAL(historyRefreshed()),
            ui->widget,SIGNAL(refreshWelcomeWidget()));

    connect(openaction,SIGNAL(historyRefreshed()),
            ui->widget,SIGNAL(refreshWelcomeWidget()));

    connect(ui->widget,SIGNAL(needToOpenFile(QString)),
            openaction,SLOT(openDataFileSlot(QString)));

    connect(openaction,SIGNAL(fileOpened(QString)),
            ui->widget,SLOT(onFileOpened()));

    /// Undo
    ClabsoToolButton *toolButton_undo = new ClabsoToolButton(groupBox_begin);
    gridLayout_begin->addWidget(toolButton_undo, 0, column++, maxRow, 1);
    toolButton_undo->setPopupMode(QToolButton::DelayedPopup);
    toolButton_undo->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    toolButton_undo->setAutoRaise(true);
    toolButton_undo->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    toolButton_undo->setIconSize(bigButtonIconSize);
    toolButton_undo->setFocusPolicy(Qt::NoFocus);

//    QAction*undoaction = clabso_ft_factory::instance()->getStack()->createUndoAction(toolButton_undo,tr("Undo"));
    QAction*undoaction = new QAction("Undo"); /*ClabsoControlCenter::instance()->createUndoAction(toolButton_undo,tr("Undo"));*/
    undoaction->setObjectName("Undo");
    undoaction->setIcon(QIcon(":/img/undo"));
    toolButton_undo->setDefaultAction(undoaction);

    ClabsoToolButton *toolButton_redo = new ClabsoToolButton(groupBox_begin);
    gridLayout_begin->addWidget(toolButton_redo, 0, column++, maxRow, 1);
    toolButton_redo->setPopupMode(QToolButton::DelayedPopup);
    toolButton_redo->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    toolButton_redo->setAutoRaise(true);
    toolButton_redo->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    toolButton_redo->setIconSize(bigButtonIconSize);
    toolButton_redo->setFocusPolicy(Qt::NoFocus);

//    QAction*undoaction = clabso_ft_factory::instance()->getStack()->createUndoAction(toolButton_undo,tr("Undo"));
    QAction*redoaction = new QAction("Redo"); /*ClabsoControlCenter::instance()->createUndoAction(toolButton_undo,tr("Undo"));*/
    redoaction->setObjectName("Redo");
    redoaction->setIcon(QIcon(":/img/redo"));
    toolButton_redo->setDefaultAction(redoaction);

    ClabsoToolButton *toolButton_setting = new ClabsoToolButton(groupBox_begin);
    gridLayout_begin->addWidget(toolButton_setting, 0, column++, maxRow, 1);
    toolButton_setting->setPopupMode(QToolButton::DelayedPopup);
    toolButton_setting->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    toolButton_setting->setAutoRaise(true);
    toolButton_setting->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    toolButton_setting->setIconSize(bigButtonIconSize);
    toolButton_setting->setFocusPolicy(Qt::NoFocus);

    QAction*settingaction = new SystemSettingAction(QIcon(":/img/setting"),tr("Setting"),"Setting",groupBox_begin);
    toolButton_setting->setDefaultAction(settingaction);



    toolBarLayout->addSpacerItem(new QSpacerItem(20,100,QSizePolicy::Expanding));

}
