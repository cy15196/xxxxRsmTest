#ifndef FORMDATAPAGE_H
#define FORMDATAPAGE_H

#include <QWidget>
#include <QMap>
#include <QPair>

#include "topteststructdata.h"

namespace Ui {
class FormDataPage;
}
class FormResultTable;

class FormDataPage : public QWidget
{
    Q_OBJECT

public:
    explicit FormDataPage(QWidget *parent = 0);
    ~FormDataPage();

    QByteArray getChartData() const;

    QMap<int,QPair<QString,QColor>> getPortInfoMap();


    TopTest::FileData &getFileData();
    void setFileData(const TopTest::FileData &value);

    QString getResultTableContentInStr(bool sutabileSection = false);
    QString getDeviceTableContentInStr(bool sutabileSection = false);
    QString getSourceDataString();

    double getVelocity();

    QString getDeviceName();
    QString getProductName();
    QString getProcessName();

    QString getFilePath() const;
    void setFilePath(const QString &value);

    QList<QMap<QString, QVariant>> getSettingFileMapList() const;
    void setSettingFileMapList(const QList<QMap<QString, QVariant>> & mapList);

    TopTest::WaveInfo getWaveInfo();

public slots:
    void setProductName(const QString& name);
    void setVelocity(const double& velocity);

    void setFileName(const QString& name);
    void setDeviceName(const QString& name);
    void setProcessName(const QString& name);
    void setPortNum(const int& portNum);
    void setTestDateTime(const QDateTime& datetime);
    void setSampleTime(const double & sampleTime);

    void addCurveData(const QByteArray& dataArray);

    void applyProcess(const QString& processName);

    void applyDevice(const QString& deviceName);

    void applyProduct(const QString& productName);

    void applyProcess(const QMap<QString,QVariant>& processMap);

    void applyDevice(const QMap<QString,QVariant>& deviceMap);

    void applyProduct(const QMap<QString,QVariant>& productMap);

    void setPortInfos(const QMap<int,QPair<QString,QColor>>& portInfoMap);

    void printOutput(int type, int printContent);

    ///重新应用所以文件设置等
    void reapply();

    void markUnsavedChanges(bool unsaved);

    void setWaveInfo(const TopTest::WaveInfo& waveInfo);


private:

    QMap<QString,QVariant> deviceMap;
    QMap<QString,QVariant> productMap;
    QMap<QString,QVariant> progressMap;

    QList<FormResultTable*> resultTableList;

    void init();

    TopTest::FileData fileData;

    Ui::FormDataPage *ui;

    QString filePath;
};

#endif // FORMDATAPAGE_H
