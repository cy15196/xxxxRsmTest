#include "formfilesetting.h"
#include "ui_formfilesetting.h"

#include <QToolButton>
#include "rsmcoresignalenum.h"
#include "formfilesettingbasewidget.h"

#include "formproductcontent.h"
#include "formdevicecontent.h"
#include "formprogresshandle.h"
#include "formexchangecontent.h"

FormFileSetting::FormFileSetting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormFileSetting)
{
    ui->setupUi(this);
    init();
}

FormFileSetting::~FormFileSetting()
{
    delete ui;
}

void FormFileSetting::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }

    QWidget::changeEvent(event);
}

void FormFileSetting::onChangePage(int pageEnum)
{
    if(pageEnum < Tab_FileSetting_Product ||
            pageEnum > Tab_FileSetting_Exchange)
        return;
    ui->stackedWidget->setCurrentIndex(pageEnum - Tab_FileSetting_Product);
}

void FormFileSetting::onReloadSystemSetting()
{
    foreach (FormFileSettingBaseWidget* settingPage, this->findChildren<FormFileSettingBaseWidget* >()) {
        settingPage->reloadSystemSetting();
    }
}

void FormFileSetting::init()
{
    QWidget* tabGroup = ui->widget_tab->addGroup("tabGroup");
    ui->widget_tab->setTranslationContext(this->metaObject()->className());

    QList<QToolButton*> toolButtonList;

    toolButtonList<< ui->widget_tab->addToolButton(tabGroup,QIcon(":/images/ic_chanpinwenjiantubiao.png"),tr("Tab_FileSetting_Product"),Tab_FileSetting_Product,"Tab_FileSetting_Product");
    toolButtonList<< ui->widget_tab->addToolButton(tabGroup,QIcon(":/images/ic_shebeiwenjiantubiao.png"),tr("Tab_FileSetting_Device"),Tab_FileSetting_Device,"Tab_FileSetting_Device");
    toolButtonList<< ui->widget_tab->addToolButton(tabGroup,QIcon(":/images/ic_gongyiwenjiantubiao.png"),tr("Tab_FileSetting_Process"),Tab_FileSetting_Process,"Tab_FileSetting_Process");
    toolButtonList<< ui->widget_tab->addToolButton(tabGroup,QIcon(":/images/ic_gongyiwenjiantubiao.png"),tr("Tab_FileSetting_Exchange"),Tab_FileSetting_Exchange,"Tab_FileSetting_Exchange");

    foreach (QToolButton* button, toolButtonList) {
        button->setCheckable(true);
        button->setAutoExclusive(true);
    }

    toolButtonList.first()->setChecked(true);

    ui->widget_tab->setGroupToolButtonSize(tabGroup,QSize(70,70),0.8);

    ui->stackedWidget->addWidget(new FormFileSettingBaseWidget(new FormProductContent(this)));
    ui->stackedWidget->addWidget(new FormFileSettingBaseWidget(new FormDeviceContent(this)));
    ui->stackedWidget->addWidget(new FormFileSettingBaseWidget(new FormProgressHandle(this)));
    FormExchangeContent* exchangePage =  new FormExchangeContent(this);
    ui->stackedWidget->addWidget(exchangePage);

    connect(exchangePage,SIGNAL(needToReloadSystemSettings()),
            this,SLOT(onReloadSystemSetting()));

    connect(ui->widget_tab,SIGNAL(toolButtonToggled(int)),
            this,SLOT(onChangePage(int)));

}
