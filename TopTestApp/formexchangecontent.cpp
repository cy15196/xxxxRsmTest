#pragma execution_character_set("utf-8")


#include "formexchangecontent.h"
#include "ui_formexchangecontent.h"

#include <QFileDialog>
#include <QSettings>
#include <QDateTime>
#include <QDebug>

FormExchangeContent::FormExchangeContent(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormExchangeContent)
{
    ui->setupUi(this);

    ui->label_state->setVisible(false);
    ui->progressBar->setVisible(false);
}

FormExchangeContent::~FormExchangeContent()
{
    delete ui;
}

void FormExchangeContent::loadSettingFileToSystem(const QString &fileName)
{
    QSettings appSetting(QCoreApplication::organizationName(),QCoreApplication::applicationName());
    QSettings iniSetting(fileName,QSettings::IniFormat);
    qDebug()<<"this ini file is create at"<<iniSetting.value("SaveDateTime").toDateTime();
    appSetting.beginGroup("FileSettings");
    foreach (QString key, iniSetting.childGroups()) {
        qDebug()<<"file type"<<key;

        iniSetting.beginGroup(key);
        appSetting.beginGroup(key);

        foreach (QString name, iniSetting.childKeys()) {
            if(!appSetting.contains(name))
            {
                appSetting.setValue(name,iniSetting.value(name));
                qDebug()<<"begin to add setting"<<key<<name<<"from file to system";
            }
        }
        iniSetting.endGroup();
        appSetting.endGroup();
    }
    appSetting.endGroup();

    emit this->needToReloadSystemSettings();
}

void FormExchangeContent::saveSettingToFile(const QString &fileName)
{
    QFile::remove(fileName);

    QSettings appSetting(QCoreApplication::organizationName(),QCoreApplication::applicationName());
    QSettings iniSetting(fileName,QSettings::IniFormat);
    appSetting.beginGroup("FileSettings");
    iniSetting.setValue("SaveDateTime",QDateTime::currentDateTime());
    foreach (QString key, appSetting.childGroups()) {
        qDebug()<<"begin type"<<key;
        appSetting.beginGroup(key);
        iniSetting.beginGroup(key);
        foreach(QString name, appSetting.childKeys())
        {
            qDebug()<<"save value of"<<name;
            iniSetting.setValue(name,appSetting.value(name));
        }
        appSetting.endGroup();
        iniSetting.endGroup();
    }
    appSetting.endGroup();
}

void FormExchangeContent::updateProgress(const QString &state, int progress)
{
    ui->label_state->setVisible(true);
    ui->progressBar->setVisible(true);

    ui->label_state->setText(state);
    ui->progressBar->setValue(progress);
}

void FormExchangeContent::on_toolButton_import_clicked()
{
    QStringList fileList = QFileDialog::getOpenFileNames(
                this,tr("Load Setting From File"),"D:/",
                "Rsm files (*.rsm *.rsf)");

    if(fileList.isEmpty())
        return;

    for(int i=0; i<fileList.count(); i++) {
        updateProgress(tr("loading"), i * 100.0 / fileList.size());
        loadSettingFileToSystem(fileList.at(i));
    }

    updateProgress(tr("Load Success"), 100);
}

void FormExchangeContent::on_toolButton_export_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(
                this,tr("Export Setting To File"),"D:/",
                "Rsm files (*.rsf)");

    if(fileName.isEmpty())
        return;

    updateProgress(tr("Exporting"), 20);

    saveSettingToFile(fileName);

    updateProgress(tr("Export Success"), 100);
}
