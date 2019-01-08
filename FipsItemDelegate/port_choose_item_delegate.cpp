#include "port_choose_item_delegate.h"

#include <QTableWidget>
#include <QCheckBox>
#include <QComboBox>
#include <QListWidget>
#include <QLineEdit>
#include <QDebug>
#include <QPainter>
#include <QApplication>

BoolComboBox::BoolComboBox(QWidget *parent)
    :QComboBox(parent)
{
    this->addItems({"False","True"});
}

BoolComboBox::~BoolComboBox()
{

}

bool BoolComboBox::boolValue() const
{
    return this->currentIndex();
}

void BoolComboBox::setBoolValue(bool value)
{
    this->setCurrentIndex(value);
}

DoubleSpin::DoubleSpin(QWidget *parent)
    :QDoubleSpinBox(parent)
{

}

DoubleSpin::~DoubleSpin()
{

}

double DoubleSpin::doubleValue() const
{
    return value();
}

void DoubleSpin::setDoubleValue(double value)
{
    this->setMaximum(1e10);
    this->setMinimum(-1e10);
    this->setValue(value);
}

MultiComboBox::MultiComboBox(QWidget *parent)
    :QComboBox(parent)
{
    qDebug()<<"MultiComboBox: new";
    pListWidget = new QListWidget(parent);
    pLineEdit = new QLineEdit(parent);
    this->setLineEdit(pLineEdit);
    this->setModel(pListWidget->model());
    this->setView(pListWidget);
    bSelected = false;
    populateList();
}

MultiComboBox::~MultiComboBox()
{

}

QString MultiComboBox::valueStr() const
{
    qDebug()<<"getValueStr";
    return pLineEdit->text();
}

void MultiComboBox::setValueStr(QString list)
{
    qDebug()<<"setValueStr"<<list;
    bSelected = true;
    pLineEdit->setText(list);
    foreach (QString portStr, list.split(";")) {
        bool ok;
        int port = portStr.toInt(&ok);
        if(ok)
        {
            QListWidgetItem *pItem = pListWidget->item(port-1);
            QWidget *pWidget = pListWidget->itemWidget(pItem);
            QCheckBox *pCheckBox = (QCheckBox *)pWidget;
            pCheckBox->setChecked(true);
        }
    }
    bSelected = false;
}

void MultiComboBox::setEditorData(const QStringList &portList)
{
    foreach (QString portStr, portList) {
        int port = portStr.toInt();
        QListWidgetItem *item = pListWidget->item(port-1);
        pListWidget->itemWidget(item);
    }
}

QList<int> MultiComboBox::getPortIntList()
{
    return {};
}

void MultiComboBox::setPortStringMap(const QMap<int, QString> &portMap)
{

}

void MultiComboBox::onStateChanged(int state)
{
    bSelected = true;
    QString strSelectedData("");
    strSelectedText.clear();
    QCheckBox *pSenderCheckBox = qobject_cast<QCheckBox *>(sender());
    int nCount = pListWidget->count();
    for (int i = 0; i < nCount; ++i)
    {
        QListWidgetItem *pItem = pListWidget->item(i);
        QWidget *pWidget = pListWidget->itemWidget(pItem);
        QCheckBox *pCheckBox = (QCheckBox *)pWidget;
        if (pCheckBox->isChecked())
        {
                QString strText = pCheckBox->text();
                strSelectedData.append(strText).append(";");
        }
         //所点击的复选框
        if (pSenderCheckBox == pCheckBox)
        {
                        int nData = pItem->data(Qt::UserRole).toInt();
            qDebug() << QString("I am sender...id : %1").arg(nData);
        }
    }
    if (strSelectedData.endsWith(";"))
        strSelectedData.remove(strSelectedData.count() - 1, 1);
    if (!strSelectedData.isEmpty())
    {
        //ui.comboBox->setEditText(strSelectedData);
        strSelectedText = strSelectedData;
        pLineEdit->setText(strSelectedData);
        pLineEdit->setToolTip(strSelectedData);
    }
    else
    {
        pLineEdit->clear();
        //ui.comboBox->setEditText("");
    }
    bSelected = false;
}

void MultiComboBox::onTextChanged(const QString &text)
{
//    qDebug()<<"onTextChanged"<<text;
    if (!bSelected)
        pLineEdit->setText(strSelectedText);
}

void MultiComboBox::populateList()
{
    for (int i = 1; i < 17; ++i)
    {
        QListWidgetItem *pItem = new QListWidgetItem(pListWidget);
        pListWidget->addItem(pItem);
        pItem->setData(Qt::UserRole, i);
        QCheckBox* pCheckBox = new QCheckBox(this);
        pCheckBox->setText(QStringLiteral("%1").arg(i));
        pListWidget->addItem(pItem);
        pListWidget->setItemWidget(pItem, pCheckBox);
        connect(pCheckBox, SIGNAL(stateChanged(int)), this, SLOT(onStateChanged(int)));
        pLineEdit->setReadOnly(true);                                                                                                                    //ui.comboBox->setEditable(true);
        connect(pLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(onTextChanged(const QString &)));
    }
}

PortChooseItemDelegate::PortChooseItemDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{

}

PortChooseItemDelegate::~PortChooseItemDelegate()
{

}

void PortChooseItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
//    QStyledItemDelegate::paint(painter,option,index);
    painter->setFont(option.font);
    QString str = index.data(Qt::DisplayRole).toString();
    QApplication::style()->drawItemText(painter,
                                        QRect(option.rect.x() + 4,
                                              option.rect.y(),
                                              option.rect.width(),
                                              option.rect.height()),
                                        Qt::AlignLeft | Qt::AlignVCenter,
                                        QPalette(QColor(255, 255, 255, 255)),
                                        true,
                                        str);
}

QWidget *PortChooseItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    MultiComboBox *comboBox = new MultiComboBox(parent);
//    comboBox->setValueStr(index.data());
//    comboBox->setPortStringMap({});
    return comboBox;
}

void PortChooseItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    dynamic_cast<MultiComboBox *>(editor)->setValueStr(index.data().toString());
}

void PortChooseItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    model->setData(index,dynamic_cast<MultiComboBox *>(editor)->valueStr(),Qt::DisplayRole);
}





