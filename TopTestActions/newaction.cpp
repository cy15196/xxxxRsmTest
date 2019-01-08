#include "newaction.h"
#include "openaction.h"

#include <QFileDialog>
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QSettings>

#include "toptestcore.h"
#include "createdialog.h"
#include "topteststructdata.h"
#include <QDateTime>
#include <QDebug>


NewAction::NewAction(const QIcon &icon, const QString& text, const QString& objName , QObject *parent )
    :ClabsoEnableAction(icon,text,objName,parent)
{

}

void NewAction::onTriggered(bool)
{
    TopTest::ProjectInfo info;
    CreateDialog dialog(&info);
    dialog.exec();
    if(info.isValid)
    {
        qDebug()<<info.projectMap;
        createEmptyFile(info);
    }
}

bool NewAction::createEmptyFile(const TopTest::ProjectInfo& info)
{
    if(info.name.isEmpty()||info.path.isEmpty())
        return false;
    QDir folder(info.path);
    if(!folder.exists())
        return false;

    ///create folder
    QFileInfo file(info.name);
    folder.mkdir(file.baseName());
    folder.cd(file.baseName());

    ///create file
    if(info.type==0)
    {
        QString suffix = TopTestCore::instance()->getDataFileSuffix();

        QString fileName = info.name;
        if(!fileName.endsWith(suffix))
            fileName+=suffix;

        fileName = folder.path()+"/"+fileName;

        QFile newFile(fileName);
        if(!newFile.open(QFile::WriteOnly))
        {
            QMessageBox::critical(0,
                                  tr("Error"),
                                  tr("Create file %1 failed!").arg(newFile.fileName()));
            return false;
        }
        newFile.close();

        QSettings dataFile(fileName,QSettings::IniFormat);
        dataFile.beginGroup("FileInfo");
        dataFile.setValue("FileType","TopTestDataFile");
        dataFile.setValue("Version",TopTestCore::instance()->getDataFileVersion());
        dataFile.setValue("CreateTime",QDateTime::currentDateTime().toString(Qt::ISODate));
        dataFile.endGroup();

        dataFile.beginGroup("ProjectInfo");
        foreach (QString key, info.projectMap.keys()) {
            dataFile.setValue(key,info.projectMap.value(key));
        }
        dataFile.endGroup();

        OpenAction::refreshHistoryList(fileName,true);

        emit this->historyRefreshed();

    }

    return true;
}
