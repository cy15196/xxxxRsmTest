#include <QtTest>

// add necessary includes here

#include "dataiomanager.h"

#define testFileName "D:/testDataIo.tpd"

class testDataIO : public QObject
{
    Q_OBJECT

public:
    testDataIO();
    ~testDataIO();

private slots:
    void test_writeAndRead();

};

testDataIO::testDataIO()
{

}

testDataIO::~testDataIO()
{

}

void testDataIO::test_writeAndRead()
{
    DataIOManager manager;
    TopTestFileDataMap map;
    QList<double> timeList = {0.0, 0.1, 0.2, 0.3, 0.4, 0.5};
    map.insert("Time",timeList);
    for(int i=1;i<=8;i++)
    {
        QList<double> portData;
        for(int j=0;j<timeList.size();j++)
        {
            portData.append(timeList.at(j)+i);
        }
        map.insert("Port"+QString::number(i),portData);
    }
    manager.saveDataToFile(map,testFileName);


    manager.readDataFile(testFileName);
}


QTEST_APPLESS_MAIN(testDataIO)

#include "tst_testdataio.moc"
