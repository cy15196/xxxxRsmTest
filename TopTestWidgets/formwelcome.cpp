#include "formwelcome.h"
#include "ui_formwelcome.h"

#include "historylabel.h"

#include <QSettings>
#include <QFile>
#include <qmath.h>

#include "clabso_ft_dpi.h"

FormWelcome::FormWelcome(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormWelcome)
{
    ui->setupUi(this);
    init();
}

FormWelcome::~FormWelcome()
{
    delete ui;
}

void FormWelcome::init()
{
    ui->label_dataVisual->setPixmap(QPixmap(":/img/chart2.png").scaledToWidth(ClabsoFTDpi::convertSize(800)));
    ui->textBrowser_describe->setText("No File Selected");
    refreshHistoryList();
    ui->listWidget_history->setCurrentItem(ui->listWidget_history->item(0));
}

void FormWelcome::showCurrentFileInfo(const QString &filePath)
{
    ui->label_dataVisual->setPixmap(
                QPixmap(":/img/chart2.png").scaledToWidth(qMax(ui->label_dataVisual->width(),(int)ClabsoFTDpi::convertSize(800))));
    ui->textBrowser_describe->clear();

    QSettings dataFile(filePath,QSettings::IniFormat);
    dataFile.beginGroup("FileInfo");
    foreach (QString key, dataFile.allKeys()) {
        ui->textBrowser_describe->append(
                    key+":\t"+
                    dataFile.value(key).toString());
    }
    dataFile.endGroup();
}

void FormWelcome::on_listWidget_history_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    HistoryLabel* label = qobject_cast<HistoryLabel*>(ui->listWidget_history->itemWidget(current));
    if(label)
        showCurrentFileInfo(label->getFile());
}

void FormWelcome::refreshHistoryList()
{
    ui->listWidget_history->clear();

    QSettings appSetting(QApplication::organizationName(), QApplication::applicationName());
    QStringList historyList = appSetting.value("History").toStringList();

    for (QString history : historyList)
    {

        if(QFile::exists(history))
        {

            QListWidgetItem *item = new QListWidgetItem();
            ui->listWidget_history->addItem(item);
            HistoryLabel *widget = new HistoryLabel(history, ui->listWidget_history);
    //        connect(widget, SIGNAL(fileLose(QString)), this, SLOT(onFileLose(QString)), Qt::QueuedConnection);
            ui->listWidget_history->setItemWidget(item, widget);
            QSize size = widget->sizeHint();
            size.setWidth(0);
            item->setSizeHint(size);
        }
        else
            appSetting.remove(history);
    }
}


void FormWelcome::on_listWidget_history_itemDoubleClicked(QListWidgetItem *item)
{
    HistoryLabel* label = qobject_cast<HistoryLabel*>(ui->listWidget_history->itemWidget(item));
    if(label)
        emit this->needToOpenFile(label->getFile());
}
