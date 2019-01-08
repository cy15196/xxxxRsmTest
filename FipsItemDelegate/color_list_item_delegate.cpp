#include "color_list_item_delegate.h"
#include <QComboBox>
#include <QPainter>

namespace Fips
{

namespace __colorlstitemdelegateprivate_
{

ColorItemDelegate::ColorItemDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{

}

ColorItemDelegate::~ColorItemDelegate()
{

}

void ColorItemDelegate::paint(QPainter *painter,
                              const QStyleOptionViewItem &option,
                              const QModelIndex &index) const
{
    painter->setBrush(QBrush(QColor(index.data().toString()), Qt::SolidPattern));
    painter->drawRect(option.rect);
}

}

}


Fips::ColorListItemDelegate::ColorListItemDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
    for (int i=Qt::black; i<Qt::transparent; ++i)
        colorList << QColor(static_cast<Qt::GlobalColor>(i)).name();
}

Fips::ColorListItemDelegate::~ColorListItemDelegate()
{

}

void Fips::ColorListItemDelegate::paint(QPainter *painter,
                                        const QStyleOptionViewItem &option,
                                        const QModelIndex &index) const
{
    painter->setBrush(QBrush(QColor(index.data().toString()), Qt::SolidPattern));
    painter->drawRect(option.rect);
}

QWidget *Fips::ColorListItemDelegate::createEditor(QWidget *parent,
                                                   const QStyleOptionViewItem &/*option*/,
                                                   const QModelIndex &/*index*/) const
{
    QComboBox *comboBox = new QComboBox(parent);
    comboBox->setObjectName("FipsColorListItemDelegateComboBox");
    comboBox->setStyleSheet("#FipsColorListItemDelegateComboBox { background-color : rgba(0,0,0,0); }");
    connect(comboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(onCurrentIndexChanged(QString)));
    comboBox->setItemDelegate(new Fips::__colorlstitemdelegateprivate_::ColorItemDelegate(parent));
    comboBox->addItems(colorList);
    return comboBox;
}

void Fips::ColorListItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    dynamic_cast<QComboBox *>(editor)->setCurrentIndex(colorList.indexOf(index.data().toString()));
}

void Fips::ColorListItemDelegate::setModelData(QWidget *editor,
                                               QAbstractItemModel *model,
                                               const QModelIndex &index) const
{
    QString str = dynamic_cast<QComboBox *>(editor)->currentText();
    if (colorList.indexOf(str) < 0)
        return;
    model->setData(index, str);
}

void Fips::ColorListItemDelegate::onCurrentIndexChanged(const QString &text)
{
    QComboBox *comboBox = dynamic_cast<QComboBox *>(sender());
    if (comboBox == FIPS_NULLPTR)
        return;
    if (text.isEmpty())
        comboBox->setStyleSheet("#FipsColorListItemDelegateComboBox { background-color : rgba(0,0,0,0); }");
    else
        comboBox->setStyleSheet("#FipsColorListItemDelegateComboBox { background-color : " + text + "; }");
}
