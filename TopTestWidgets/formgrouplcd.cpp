#include "formgrouplcd.h"

#include <QGridLayout>
#include "formnolcd.h"

FormGroupLCD::FormGroupLCD(QWidget *parent) :
    QWidget(parent)
{
    rowCount = 2;
    columnCount = 8;
    portNum = rowCount * columnCount;

    QGridLayout* lcdLayout = new QGridLayout();
    this->setLayout(lcdLayout);

    int nowNo = 1;
    for(int i = 0; i< rowCount ; i++)
        for( int j = 0; j<columnCount; j++)
        {
            FormNoLCD * noLcd = new FormNoLCD(this);
            noLcd->setNo(nowNo);
            noToLCDMap[nowNo] = noLcd;
            lcdLayout->addWidget(noLcd,i,j);
            nowNo++;
        }
}

FormGroupLCD::~FormGroupLCD()
{

}

void FormGroupLCD::showLCDValueList(const QList<double> &portValues)
{
    for(int i = 0; (i< portValues.count()) && (i < portNum) ; i++ )
    {
        FormNoLCD* lcdNumber = noToLCDMap[i+1];
        if(lcdNumber)
        {
            lcdNumber->setLCDValue(portValues[i]);
        }
    }

}

int FormGroupLCD::getColumnCount() const
{
    return columnCount;
}

void FormGroupLCD::setColumnCount(int value)
{
    columnCount = value;
}

int FormGroupLCD::getPortNum() const
{
    return portNum;
}

void FormGroupLCD::setPortNum(int value)
{
    portNum = value;

    foreach (int portNo , noToLCDMap.keys() ) {
        if(portNo > portNum)
            noToLCDMap[portNo]->setLCDVisiable(false);
        else
            noToLCDMap[portNo]->setLCDVisiable(true);
    }
}
