#include "toptestsettingmanager.h"

#include <QFileInfo>
#include <QDir>
#include <QTranslator>
#include <QCoreApplication>
#include <QString>
#include <QMap>
#include <QDebug>
#include <QSettings>

#define ApplicationDirPath QString(QCoreApplication::applicationDirPath()+"/")

TopTestSettingManager *TopTestSettingManager::manager = 0;

struct TopTestSettingManagerPrivate
{
    void changeLanguage(const QString &languageTag);
    QMap<QString,QTranslator*> translatorMap;
};

void TopTestSettingManagerPrivate::changeLanguage(const QString &languageTag)
{
//    QTranslator translator;
//    bool open = translator.load(languageTag,"translations");
//    bool install = qApp->installTranslator(&translator);
//    qDebug()<<"load language file "<<languageTag<<open<<install;

    qDebug()<<"TopTestSettingManagerPrivate::changeLanguage"<<languageTag;
    QString fileName = "Rsm_" + languageTag;

    QDir dir(ApplicationDirPath + "translations");
    if (!dir.exists())
        return;

    dir.setFilter(QDir::Files | QDir::NoSymLinks);
    dir.setNameFilters(QStringList() << QString("*%1.qm").arg(languageTag));
    QFileInfoList list = dir.entryInfoList();

    for (QFileInfo info : list)
    {
        if (info.baseName() != fileName)
        {
            QTranslator* translator = translatorMap[info.baseName()];
            if (!translator)
                translator = new QTranslator(qApp);
            bool open = translator->load(info.baseName(),ApplicationDirPath+ "translations");
            bool install = qApp->installTranslator(translator);
            qDebug() << "load language file " << fileName << open << install;

            if(open && install)
                translatorMap.insert(fileName, translator);
            else
                translatorMap.remove(fileName);
        }
    }

    QTranslator* translator = translatorMap[fileName];
    if (!translator)
        translator = new QTranslator(qApp);
    bool open = translator->load(fileName,ApplicationDirPath+ "translations");
    bool install = qApp->installTranslator(translator);
    qDebug() << "load language file " << fileName << open << install;

    if(open && install)
        translatorMap.insert(fileName, translator);
    else
        translatorMap.remove(fileName);
}


TopTestSettingManager *TopTestSettingManager::instance()
{
    if (!manager)
    {
        manager = new TopTestSettingManager(qApp);
        manager->loadFromSystemSettings();
    }
    return manager;
}

TopTestSettingManager::TopTestSettingManager(QObject *parent)
    : QObject(parent),d_ptr(new TopTestSettingManagerPrivate)
{

}

TopTestSettingManager::~TopTestSettingManager()
{
    delete d_ptr;
}

QVariant TopTestSettingManager::getSystemSettingKeyValue(const QString &settingTypeName)
{
    return currentSettingMap[settingTypeName];
}

void TopTestSettingManager::setSystemSettingKeyValue(const QString &settingTypeName, const QVariant &value)
{
    currentSettingMap[settingTypeName] = value;
}

void TopTestSettingManager::changeToLanguage(const QString &lanTag)
{
    d_ptr->changeLanguage(lanTag);
}

void TopTestSettingManager::loadFromSystemSettings()
{
    QSettings appSetting(qApp->organizationName(),qApp->applicationName());
    appSetting.beginGroup("SystemSetting");

    foreach (QString key, appSetting.childKeys()) {
        currentSettingMap.insert(key,appSetting.value(key));
    }

    appSetting.endGroup();
}

void TopTestSettingManager::saveToSystemSettings()
{
    QSettings appSetting(qApp->organizationName(),qApp->applicationName());
    appSetting.beginGroup("SystemSetting");

    appSetting.remove("");

    foreach (QString key, currentSettingMap.keys()) {
        appSetting.setValue(key,currentSettingMap[key]);
    }

    appSetting.endGroup();
}

void TopTestSettingManager::initDefaultValues()
{
    currentSettingMap[""];
}
