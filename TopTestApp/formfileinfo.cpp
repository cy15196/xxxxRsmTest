#pragma execution_character_set("utf-8")

#include "formfileinfo.h"
#include "ui_formfileinfo.h"

#include <QDebug>

FormFileInfo::FormFileInfo(QWidget *parent) :
    DialogBase(parent),
    ui(new Ui::FormFileInfo)
{
    ui->setupUi(this);
    init();
}

FormFileInfo::~FormFileInfo()
{
    delete ui;
}

void FormFileInfo::onButtonClicked(int role)
{
    if(role == QDialogButtonBox::Ok)
    {
        emit this->fileInfoChanged(getFileInfoFromWidget());
    }

}

int FormFileInfo::standerButtons()
{
    return QDialogButtonBox::Ok;
}

void FormFileInfo::fillCurrentFileInfo(const QVariantMap &fileInfo)
{
    foreach (QString objName, fileInfo.keys()) {
        QWidget* childWidget = this->findChild<QWidget*>(objName);
        if(!childWidget)
            continue;

        QLineEdit* lineEdit = qobject_cast<QLineEdit*>(childWidget);
        if(lineEdit)
        {
            lineEdit->setText(fileInfo.value(objName).toString());
            continue;
        }

        QPlainTextEdit* PlainTextEdit = qobject_cast<QPlainTextEdit*>(childWidget);
        if(PlainTextEdit)
        {
            PlainTextEdit->setPlainText(fileInfo.value(objName).toString());
            continue;
        }

        QDateTimeEdit* DateTimeEdit = qobject_cast<QDateTimeEdit*>(childWidget);
        if(DateTimeEdit)
        {
            DateTimeEdit->setDateTime(fileInfo.value(objName).toDateTime());
            continue;
        }

    }
}

void FormFileInfo::init()
{
    ui->DateTime->setDateTime(QDateTime::currentDateTime());
    ui->Line->setEnabled(false);
//    ui->Comment->setPlaceholderText(tr(""));
}

QVariantMap FormFileInfo::getFileInfoFromWidget()
{
    QVariantMap ret;

    foreach (QWidget* childWidget, this->findChildren<QWidget*>()) {
        QLineEdit* lineEdit = qobject_cast<QLineEdit*>(childWidget);
        if(lineEdit)
        {
            ret.insert(childWidget->objectName(),lineEdit->text());
            continue;
        }

        QPlainTextEdit* PlainTextEdit = qobject_cast<QPlainTextEdit*>(childWidget);
        if(PlainTextEdit)
        {
            ret.insert(childWidget->objectName(),PlainTextEdit->toPlainText());
            continue;
        }

        QDateTimeEdit* DateTimeEdit = qobject_cast<QDateTimeEdit*>(childWidget);
        if(DateTimeEdit)
        {
            ret.insert(childWidget->objectName(),DateTimeEdit->dateTime());
            continue;
        }

    }

    qDebug()<<"FormFileInfo::getFileInfoFromWidget()"<<ret;

    return ret;
}
