#include "checktoolbutton_item_delegate.h"

#include <QPainter>
#include <QColor>
#include <QDebug>

#include <QToolButton>
#include <QTableWidget>
#include <QTableWidgetItem>


ToolButtonItemDelegate::ToolButtonItemDelegate(QTableWidget *table)
    :QStyledItemDelegate(table),table(table)
{

}

ToolButtonItemDelegate::~ToolButtonItemDelegate()
{

}

void ToolButtonItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyledItemDelegate::paint(painter,option,index);
    if(table)
    {
        QToolButton* toolButton = dynamic_cast<QToolButton*> (table->indexWidget(index)) ;
        if(!toolButton)
        {
            toolButton = new QToolButton(table);
            toolButton->setCheckable(checkable);
            if(toolButton->isCheckable())
            {
                toolButton->setChecked(true);
                toolButton->setIcon(QIcon(iconPathChecked));
                connect(toolButton,SIGNAL(toggled(bool)),
                        SLOT(onToolButtonClicked(bool)));
                bool checked = index.data().toBool();
                toolButton->setChecked(checked);
            }
            else
                toolButton->setText(index.data().toString());
            table->setIndexWidget(index,toolButton);
        }
    }

}

QTableWidget *ToolButtonItemDelegate::getTable() const
{
    return table;
}

void ToolButtonItemDelegate::setTable(QTableWidget *value)
{
    table = value;
}

void ToolButtonItemDelegate::onToolButtonClicked(bool checked)
{
    QToolButton* button = dynamic_cast<QToolButton*>(sender());
    if(button)
    {
        if(checked)
            button->setIcon(QIcon(iconPathChecked));
        else
            button->setIcon(QIcon(iconPathUnchecked));
    }
}

bool ToolButtonItemDelegate::getCheckable() const
{
    return checkable;
}

void ToolButtonItemDelegate::setCheckable(bool value)
{
    checkable = value;
}

QString ToolButtonItemDelegate::getIconPathUnchecked() const
{
    return iconPathUnchecked;
}

void ToolButtonItemDelegate::setIconPathUnchecked(const QString &value)
{
    iconPathUnchecked = value;
}

QString ToolButtonItemDelegate::getIconPathChecked() const
{
    return iconPathChecked;
}

void ToolButtonItemDelegate::setIconPathChecked(const QString &value)
{
    iconPathChecked = value;
}


