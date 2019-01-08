#include "dialogsavedatafile.h"
#include "ui_dialogsavedatafile.h"

#include "toptestsettingmanager.h"
#include "topteststructdata.h"

#include <QFileDialog>
#include <QDir>
#include <QRegularExpression>
#include <QDebug>

DialogSaveDataFile::DialogSaveDataFile(QWidget *parent) :
    DialogBase(parent),
    ui(new Ui::DialogSaveDataFile)
{
    ui->setupUi(this);

    connect(ui->lineEdit_name,SIGNAL(textChanged(QString)),
            this,SLOT(onFileNameChanged()));
    connect(ui->lineEdit_path,SIGNAL(textChanged(QString)),
            this,SLOT(onFileNameChanged()));

    ui->label_pic->setPixmap(QPixmap(":/images/ic_baocunwenjiantubiao.png")
                             .scaled(ui->label_pic->size()));
    ui->lineEdit_path->setEnabled(false);
    ui->lineEdit_path->setText("D:/");
    TopTestSettingManager::instance()->loadFromSystemSettings();
    QString workDir = TopTestSettingManager::instance()->getSystemSettingKeyValue("WorkDir:").toString();

    if(QDir(workDir).exists())
    {
        ui->lineEdit_path->setText(workDir);
    }

}

DialogSaveDataFile::~DialogSaveDataFile()
{
    delete ui;
}

void DialogSaveDataFile::setFileInfo(TopTest::RecordInfoData *infoData)
{
    if(!infoData)
        return;

    this->infoData = infoData;

    setCurrentProductName(this->infoData->Product);
    setCurrentFileName(this->infoData->fileName);



}

void DialogSaveDataFile::setCurrentProductName(const QString &path)
{

    if(path.isEmpty())
    {
        qDebug()<<"DialogSaveDataFile::setCurrentProductName is invalid"<<path;
        return;
    }

    QString realPath = path;

    if(realPath.startsWith("BuiltIn:"))
        realPath = path.right(path.length() -8 );

    TopTestSettingManager::instance()->loadFromSystemSettings();
    QString workDir = TopTestSettingManager::instance()->getSystemSettingKeyValue("WorkDir:").toString();

    if(QDir(workDir).exists())
    {
        QDir(workDir).mkdir(realPath);
        ui->lineEdit_path->setText(workDir+"/"+realPath);
    }

}

void DialogSaveDataFile::setCurrentFileName(const QString &fileName)
{
    ui->lineEdit_name->setText(fileName);
}

void DialogSaveDataFile::onButtonClicked(int role)
{
    if(role == QDialogButtonBox::Ok)
    {
        emit this->saveCurrentFileToPath(saveFilePath);
    }
}

void DialogSaveDataFile::on_pushButton_changeDir_clicked()
{
    QString dirPath = QFileDialog::getExistingDirectory(this,tr("Choose dir path"),"D:/");
    if(!dirPath.isEmpty())
        ui->lineEdit_path->setText(dirPath);
}

void DialogSaveDataFile::onFileNameChanged()
{
    ui->lineEdit_name->blockSignals(true);
    ui->lineEdit_path->blockSignals(true);

    QString tempPath = ui->lineEdit_path->text() + "/" + ui->lineEdit_name->text();

    if(!tempPath.endsWith(".rsm"))
        tempPath += ".rsm";

    saveFilePath = tempPath;

    if(this->infoData)
    {

        if(this->infoData->saveFilePath.isEmpty() || this->infoData->saveFilePath != saveFilePath) //new create file or save as
        {
            qDebug()<<"infoData.saveFilePath"<<infoData->saveFilePath<<"nowSavePath"<<saveFilePath;

            QFileInfo fileInfo(saveFilePath);
            QString nowFileName = fileInfo.completeBaseName();

            while(QFile::exists(fileInfo.absoluteFilePath()))
            {
                QRegularExpression rg("_(\\d+)$");
                QRegularExpressionMatch match = rg.match(nowFileName);
                if(match.hasMatch())
                {
                    QString matchText = match.captured(0);
                    qDebug()<<"match"<<nowFileName<<matchText;
                    bool ok;
                    int d = match.captured(1).toInt(&ok);
                    if(ok && d > 0)
                    {
                        nowFileName.remove(matchText);
                        nowFileName += QString("_%1").arg(d+1);
                    }
                }
                else {
                    nowFileName += QString("_%1").arg(1);
                }

                fileInfo = QFileInfo(fileInfo.absolutePath()+ "/" + nowFileName + ".rsm");

            }

            ui->lineEdit_name->setText(nowFileName);


            saveFilePath = fileInfo.absoluteFilePath();

        }
    }

    qDebug()<<"fileFilePath"<<saveFilePath;

    ui->lineEdit_name->blockSignals(false);
    ui->lineEdit_path->blockSignals(false);
}
