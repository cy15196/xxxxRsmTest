#include "dialogexceldataoutput.h"
#include "ui_dialogexceldataoutput.h"

#include <QClipboard>
#include <QDebug>
#include <QDesktopServices>
#include <QUrl>

#include <QDir>
#include <QDateTime>
#include <QMessageBox>

DialogExcelDataOutput::DialogExcelDataOutput(QWidget *parent) :
    DialogBase(parent),
    ui(new Ui::DialogExcelDataOutput)
{
    ui->setupUi(this);
    ui->label->setPixmap(QPixmap(":/images/ic_baocunwenjiantubiao.png").scaledToHeight(ui->label->height()));
}

DialogExcelDataOutput::~DialogExcelDataOutput()
{
    delete ui;
}

int DialogExcelDataOutput::standerButtons()
{
    return DialogBase::standerButtons() | QDialogButtonBox::Yes;
}

void DialogExcelDataOutput::onButtonClicked(int role)
{
    switch (role) {
    case QDialogButtonBox::Yes:
    {
        qDebug()<<"try open excel";

        QString fileName = QDir::tempPath()+"/"+qApp->applicationName() + QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss")+".CSV";
        QFile tempFile(fileName);
        if(!tempFile.open(QFile::WriteOnly))
        {
            QMessageBox::critical(this,tr("Error"),tr("Failed to create temp file"));
            return;
        }

        QTextStream ts(&tempFile);
//        ts.setCodec("UTF-8");
        ts<<QString(outputStr).replace("\t",",");

        tempFile.close();

        qDebug()<<"try to use excel to open temp csv file";
        QDesktopServices::openUrl(QUrl::fromLocalFile(fileName));
    }
    default:
        break;
    }

}

QString DialogExcelDataOutput::getOutputStr() const
{
    return outputStr;
}

void DialogExcelDataOutput::setOutputStr(const QString &value)
{
    outputStr = value;

    QClipboard *clipboard = QApplication::clipboard();   //获取系统剪贴板指针
    clipboard->setText(outputStr);		             //设置剪贴板内容</span>
}
