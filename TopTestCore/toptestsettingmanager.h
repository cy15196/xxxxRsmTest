#ifndef TOPTESTSETTINGMANAGER_H
#define TOPTESTSETTINGMANAGER_H

#include <QObject>
#include <QMap>
#include "toptestcore_global.h"
#include "toptestenum.h"

struct TopTestSettingManagerPrivate;

class TOPTESTCORESHARED_EXPORT TopTestSettingManager : public QObject
{
    Q_OBJECT
public:

    static TopTestSettingManager* instance();



    explicit TopTestSettingManager(QObject *parent = nullptr);
    ~TopTestSettingManager();

    QVariant getSystemSettingKeyValue(const QString& settingTypeName);
    void setSystemSettingKeyValue(const QString& settingTypeName,const QVariant& value);

signals:

public slots:

    void changeToLanguage(const QString& lanTag);

    ///
    /// \brief loadFromSystemSettings 从系统中加载已经保存好的系统设置值
    ///
    void loadFromSystemSettings();
    void saveToSystemSettings();

private:

    void initDefaultValues();

    QMap<QString,QVariant> currentSettingMap;

    static TopTestSettingManager *manager;

    TopTestSettingManagerPrivate * d_ptr;
};

#endif // TOPTESTSETTINGMANAGER_H
