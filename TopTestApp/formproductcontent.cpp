#pragma execution_character_set("utf-8")

#include "formproductcontent.h"
#include "ui_formproductcontent.h"

#include "clabso_ft_dpi.h"

#include <QDebug>
#include <QFileDialog>

FormProductContent::FormProductContent(QWidget *parent) :
    FileSettingWidget(parent),
    ui(new Ui::FormProductContent)
{
    ui->setupUi(this);
//    ui->label_pic->setPixmap(
//                QPixmap(":/images/ic_shangchauntupianda.png").scaled(640,480));

    connect(ui->lineEdit_name,SIGNAL(textChanged(QString)),
            this,SIGNAL(contentChanged()));
    connect(ui->textEdit,SIGNAL(textChanged()),
            this,SIGNAL(contentChanged()));

    connect(ui->widget_pic,SIGNAL(contentChanged()),
            this,SIGNAL(contentChanged()));

    ui->widget->setMaximumWidth(ClabsoFTDpi::convertSize(400));

    ui->widget_2->setMaximumSize(ClabsoFTDpi::convertSize(QSize(400,240)));

    ui->widget_pic->setPushButtonPos(QPoint(20,200));
}

FormProductContent::~FormProductContent()
{
    delete ui;
}

QString FormProductContent::getFileTypeDisplayText()
{
    return tr("产品文件设置");
}

QString FormProductContent::getFileTypeSettingKeyName()
{
    return "Product";
}

void FormProductContent::loadSettigRecord(const QVariant &recordName)
{
    QMap<QString,QVariant> recordInfoMap = readDataFromSettingFile(recordName.toString()).toMap();

    if(recordInfoMap.isEmpty())
    {
        qDebug()<<"no such record";
        return;
    }

    if(recordInfoMap.value("Name") != recordName)
    {
        qDebug()<<"record is not valid";
        return;
    }

    ui->lineEdit_name->setText(recordInfoMap["Name"].toString());
    loadWidgetContentMap(recordInfoMap);
//    ui->label_pic->setPixmap(recordInfoMap["Pic"].toString());
}

void FormProductContent::saveCurrentSettingRecord()
{
    QMap<QString,QVariant> recordInfoMap = saveWidgetContentToMap();
    //save content
    QString recordName = ui->lineEdit_name->text();
    recordInfoMap.insert("Name",recordName);
//    recordInfoMap.insert("Pic",ui->label_pic->pixmap());

    //save record
    writeDataToSettingFile(recordName,recordInfoMap);
}

void FormProductContent::setCurrentRecordName(const QString &label)
{
    ui->lineEdit_name->setText(label);
}

QString FormProductContent::getCurrentRecordName()
{
    return ui->lineEdit_name->text();
}

void FormProductContent::clearContent()
{
    ui->lineEdit_name->clear();
//    ui->label_pic->setPixmap(
//                QPixmap(":/images/ic_shangchauntupianda.png").scaled(640,480));
    ui->widget_pic->clearContent();
    ui->textEdit->clear();
}

//void FormProductContent::on_pushButton_clicked()
//{
//    QString pictureFilePath = QFileDialog::getOpenFileName(
//                this,"Choose Picture File",
//                QDir::homePath()+"/Pictures",
//                "Picture files (*.png *.jpg *.jpeg)");
//    if(!pictureFilePath.isEmpty())
//    {
////        ui->label_pic->setPixmap(
////                    QPixmap(pictureFilePath).scaled(640,480));

//        emit this->contentChanged();
//    }
//}
