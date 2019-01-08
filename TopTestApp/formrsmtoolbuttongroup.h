#ifndef FORMRSMTOOLBUTTONGROUP_H
#define FORMRSMTOOLBUTTONGROUP_H

#include <QWidget>
#include <QMap>

#include <QIcon>

class QToolButton;
class QGroupBox;

struct ToolButtonInfo{
    QIcon icon;
    QString text;
    int signal;
    QString objName;
};

class FormRSMToolButtonGroup : public QWidget
{
    Q_OBJECT

public:
    explicit FormRSMToolButtonGroup(QWidget *parent = 0);
    ~FormRSMToolButtonGroup();

    QWidget* addGroup(const QString& groupName, bool vertical = true);

    QList<QToolButton*> getGroupButtonList(const QString& groupName);


    QToolButton* addToolButton(QWidget *group,
                               const QIcon& icon,
                               const QString& text,
                               const int & signal,
                               const QString& objName = QString());

    void addSpaceItemToGroup(QWidget* group);

    void setGroupToolButtonSize(QWidget* group,const QSize& size, const double & iconScaledRatio = 1.00 );


    inline QToolButton* addToolButton(const QString& groupName,
                                      ToolButtonInfo info )
    {
        return addToolButton(nameToGroupBoxMap[groupName],
                             info.icon,
                             info.text,
                             info.signal,
                             info.objName);
    }

    inline QToolButton* addToolButton(const QString& groupName,
                               const QIcon& icon,
                               const QString& text,
                               const int & signal,
                               const QString& objName = QString())
    {
        return addToolButton(nameToGroupBoxMap[groupName],
                             icon,
                             text,
                             signal,
                             objName);
    }

//    QToolButton* getToolButton

    QString getTranslationContext() const;
    void setTranslationContext(const QString &value);

protected:
    void changeEvent(QEvent* event);


private slots:
    void onGroupToolButtonToggled(bool checked);

signals:
    void toolButtonToggled(int sigNo);
private:

    QString translationContext;

    QMap<QObject*, int> objectToSignalMap;
    QMap<QString, QWidget*> nameToGroupBoxMap;
};

#endif // FORMRSMTOOLBUTTONGROUP_H
