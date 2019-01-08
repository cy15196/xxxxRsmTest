#include "formrsmtoolbuttongroup.h"

#include <QToolButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QCoreApplication>
#include <QDebug>
#include <QSpacerItem>
#include <QBoxLayout>

FormRSMToolButtonGroup::FormRSMToolButtonGroup(QWidget *parent) :
    QWidget(parent)
{
    this->setLayout(new QVBoxLayout);
    this->layout()->setMargin(0);
    this->layout()->setSpacing(0);
}

FormRSMToolButtonGroup::~FormRSMToolButtonGroup()
{

}

QWidget *FormRSMToolButtonGroup::addGroup(const QString &groupName, bool vertical)
{
    QWidget * group= new QWidget(this);
//    group->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    if(vertical)
        group->setLayout(new QVBoxLayout);
    else
        group->setLayout(new QHBoxLayout);
    group->layout()->setMargin(0);
    group->layout()->setSpacing(0);
    group->setObjectName(groupName);
//    group->setTitle(QCoreApplication::translate("RSM",groupName.toUtf8().data()));
    this->layout()->addWidget(group);
    nameToGroupBoxMap[groupName] = group ;
    return group;
}

QList<QToolButton *> FormRSMToolButtonGroup::getGroupButtonList(const QString &groupName)
{
    QList<QToolButton *> ret;
    QWidget* widget = nameToGroupBoxMap[groupName];
    if(widget)
    {
        return widget->findChildren<QToolButton*>();
    }

    return ret;
}


QToolButton *FormRSMToolButtonGroup::addToolButton(QWidget *group, const QIcon &icon, const QString &text, const int &signal, const QString &objName)
{
    QToolButton* newToolButton = new QToolButton(group);
    newToolButton->setText(text);
    newToolButton->setIcon(icon);
    newToolButton->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    newToolButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    QString toolButtonObjName = objName;
    if(toolButtonObjName.isEmpty())
        toolButtonObjName = "toolButton_"+text;
    newToolButton->setObjectName(objName);

    group->layout()->addWidget(newToolButton);

    objectToSignalMap[newToolButton] = signal;

    connect(newToolButton,SIGNAL(clicked(bool)),
            this,SLOT(onGroupToolButtonToggled(bool)));

    return newToolButton;
}

void FormRSMToolButtonGroup::addSpaceItemToGroup(QWidget *group)
{
    QBoxLayout* boxLayout = dynamic_cast<QBoxLayout*> (group->layout());
    boxLayout->addStretch(0);
//    QSpacerItem* spaceItem= new QSpacerItem(20, 20, QSizePolicy::Preferred, QSizePolicy::Preferred);
//    group->layout()->addItem(spaceItem);
}

void FormRSMToolButtonGroup::setGroupToolButtonSize(QWidget *group, const QSize &size, const double &iconScaledRatio)
{
    foreach (QToolButton* button, group->findChildren<QToolButton*>()) {
        button->setFixedSize(size);
        button->setIconSize(size*iconScaledRatio);
    }
}

void FormRSMToolButtonGroup::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        qDebug()<<translationContext<<"FormRSMToolButtonGroup::changeEvent(QEvent *event)"<<event;

        foreach (QToolButton* button, this->findChildren<QToolButton*>()) {
            button->setText(QCoreApplication::translate(translationContext.toLatin1().data(),button->objectName().toLatin1().data()));
        }
    }

    QWidget::changeEvent(event);
}

void FormRSMToolButtonGroup::onGroupToolButtonToggled(bool checked)
{
    Q_UNUSED(checked);
    qDebug()<<"emit signal "<<objectToSignalMap.value(sender(),-1);
    emit this->toolButtonToggled(objectToSignalMap.value(sender(),-1));
}

QString FormRSMToolButtonGroup::getTranslationContext() const
{
    return translationContext;
}

void FormRSMToolButtonGroup::setTranslationContext(const QString &value)
{
    translationContext = value;
}
