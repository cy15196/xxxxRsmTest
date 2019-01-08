#include "openaction.h"
#include <QFileDialog>
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QSettings>
#include <QDebug>
#include <QApplication>

#include "toptestcore.h"

#include "chartwidget.h"

OpenAction::OpenAction(const QIcon &icon, const QString &text, const QString &objName, QObject *parent)
    :ClabsoEnableAction(icon,text,objName,parent)

{

}

void OpenAction::refreshHistoryList(const QString &currentFilePath, bool openSuccess)
{
    //read setting file to get recent case
    QSettings appSetting(QApplication::organizationName(),QApplication::applicationName());
    QStringList actionList;
    actionList = appSetting.value("History").toStringList();

    //refresh list
    actionList.removeAll(currentFilePath);
    if(openSuccess)
        actionList.push_front(currentFilePath);

    //scan computer to make sure the file is exist
    foreach (QString filePath, actionList) {
        if(!QFile::exists(filePath))
            actionList.removeAll(filePath);
    }

    //refresh the value saved in setting
    appSetting.setValue("History",actionList);
}

bool OpenAction::openProjectFile(const QString &fileName)
{

    if(fileName.isEmpty())
        return false;

    QFile newFile(fileName);
    if(!newFile.open(QFile::ReadOnly))
    {
        QMessageBox::critical(0,
                              tr("Error"),
                              tr("Open file %1 failed!").arg(newFile.fileName()));
        return false;
    }
    newFile.close();

    QSettings dataFile(fileName,QSettings::IniFormat);
    dataFile.beginGroup("FileInfo");
    QString fileType = dataFile.value("FileType").toString();
    QString fileVersion = dataFile.value("Version").toString();
    dataFile.endGroup();

    qDebug()<<fileType<<fileVersion;

    if(fileVersion>TopTestCore::instance()->getDataFileVersion())
    {
        QMessageBox::critical(0,
                              tr("Error"),
                              tr("File %1 version is too high!").arg(newFile.fileName()));
        return false;
    }


    ///get current device and process
    dataFile.beginGroup("ProjectInfo");
    dataFile.endGroup();

    ///open curve data
    dataFile.beginGroup("Data");
    QString dataFilePath = dataFile.value("CurrentData").toString();
    if(QFile::exists(dataFilePath))
    {
//        qDebug()<<ChartWidget::lastInstance();
//        ChartWidget::lastInstance()->loadDataFile(dataFilePath);
    }
    dataFile.endGroup();


    QMessageBox::information(0,
                          tr("Open file result"),
                          tr("Open file %1  successed!").arg(newFile.fileName()));

    refreshHistoryList(fileName,true);

    return true;
}

void OpenAction::onTriggered(bool)
{
    QString fileName = QFileDialog::getOpenFileName(0,
                                 tr("Open data file"),
                                 QDir::homePath(),
                                 TopTestCore::instance()->getDataFileFilter());

    openDataFileSlot(fileName);
}

void OpenAction::openDataFileSlot(const QString &filePath)
{
    if(openProjectFile(filePath))
    {
        TopTestCore::instance()->setCurrentProjectPath(filePath);
        emit this->historyRefreshed();
        emit fileOpened(filePath);
    }
}
