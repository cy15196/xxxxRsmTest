#include "clabsoenablehelper.h"
#include "clabso_ft_io.h"

ClabsoEnableHelper* ClabsoEnableHelper::helper = 0;

ClabsoEnableHelper *ClabsoEnableHelper::instance()
{
    if(helper==0)
        helper = new ClabsoEnableHelper;
    return helper;
}

void ClabsoEnableHelper::setActionEnable(const QString &className, bool enable)
{
    emit this->actionEnableChanged(className,enable);
}

ClabsoEnableHelper::ClabsoEnableHelper(QObject *parent)
    :QObject(parent)
{ }


///*********************************************************************************///////////////
QMap<QString, ClabsoEnableAction*> ClabsoEnableAction::instanceMap;
ClabsoEnableAction *ClabsoEnableAction::instance(const QString &name)
{
    return instanceMap[name];
}

ClabsoEnableAction::ClabsoEnableAction(const QIcon &icon, const QString &text, const QString&objName, QObject *parent)
    :QAction(icon,text,parent)
{
    connect(ClabsoEnableHelper::instance(),SIGNAL(actionEnableChanged(QString,bool)),
            this,SLOT(onActionEnableChanged(QString,bool)));
    connect(this,SIGNAL(triggered(bool)),SLOT(onClicked(bool)));
    QString name = objName;
    if(name.isEmpty())
        name = text;
    this->setObjectName(name);

    instanceMap[name] = this;
}

void ClabsoEnableAction::onActionEnableChanged(const QString &className, bool enable)
{
    if(className == this->metaObject()->className())
        this->setEnabled(enable);
}

void ClabsoEnableAction::onClicked(bool check)
{
    CsLogs("Class:%1 Name:%2 onTriggerd , isCheck:%3",this->metaObject()->className(),this->objectName(),check);
    onTriggered(check);
}
