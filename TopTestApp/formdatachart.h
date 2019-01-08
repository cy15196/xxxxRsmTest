#ifndef FORMDATACHART_H
#define FORMDATACHART_H

#include <QWidget>
#include <QMap>
#include "topteststructdata.h"

namespace Ui {
class FormDataChart;
}

class QLabel;

class FormDataChart : public QWidget
{
    Q_OBJECT

public:
    explicit FormDataChart(QWidget *parent = 0);
    ~FormDataChart();

    void setFileName(const QString& name);
    void setDeviceName(const QString& name);
    void setProcessName(const QString& name);
    void setProductName(const QString& name);
    void setTestDateTime(const QDateTime& datetime);
    void setSampleTime(const double& sampleTime);
    void setVelocity(const double & velocity);

    QString getDeviceName();
    QString getProductName();
    QString getProcessName();

    QDateTime getTestDateTime();
    double getVelocity();

    void setPortNum(const int& portNum);
    int getPortNum() const;

    void setCurveData(const QByteArray& data);
    void addCurveData(const double& sampleTime,const QByteArray& data);

    void addOneSampleData(const double& sampleTime,const QList<double> & portList);

    void setSliderEditable(bool enable);


    QMap<QString, QLabel *> getSettingTypeToEditMap() const;
    void setSettingTypeToEditMap(const QMap<QString, QLabel *> &value);

    ///return the result to show in result table
    // waveNo algoType  port   result
    QMap<int, QMap<int, QList<QMap<int, QVariant> > > > applyProgress(const QString& progressName);
    QMap<int, QMap<int, QList<QMap<int, QVariant> > > > applyProgress(
            const QMap<QString,QVariant>& processMap);


    QMap<int,QPair<QString,QColor>> getPortInfoMap();


    QList<double> getPortValueListInTime(const double& time);

//    void setRulerSliderPos(int timeCount);

    QString getSourceDataString();

    QWidget* getToolGroupWidget();

    void clearData();
    QByteArray getCurrentCurveData();

    QPixmap formChartPixmap();
    QPixmap formLengendPixmap();

    void setDrawWave(bool drawWave);

    void setWaveInfo(const TopTest::WaveInfo& waveInfo);
    TopTest::WaveInfo getWaveInfo();


signals:
    void currentTimeChanged(double time);

    void needToReapply();

    void timeTypeChanged(int type);
    void tempTypeChanged(int type);

//    void needShowTempSection(bool show);

public slots:
    void onToolButtonClicked(int button);

    void showTimeLineInValue(const double& value);

    void setPortInfos(const QMap<int,QPair<QString,QColor>>& portInfoMap);

    void moveSliderToTime(const double& time);
    void setSliderRange(const double&max, const double& min);

    void drawDeviceTempSection(const QString& deviceName);
    void drawDeviceTempSection(const QMap<QString,QVariant>& deviceMap);
private:

    void initFileSettings();
    void initToolButtons();

    QMap<QString,QVariant> labelValueMap;

    ///TODO: parser one progress record to Algorithm things
    ///  algoType   applyPortList     setting  valueMap
    QMap<int, QList<QPair<QList<int>, QMap<QString, double> > > >
        parserSpecialProgress(const QString& progressName);

    QMap<int, QList<QPair<QList<int>, QMap<QString, double> > > >
        parserSpecialProgress(const QMap<QString,QVariant>& processMap);

//    ///can do some parrllel in future
//    void applyOneProgressToPortList(
//            int algType,
//            QPair<QList<int>, QMap<QString, double> > oneRecordProgress );

    Ui::FormDataChart *ui;
    QMap<QString, QLabel*> settingTypeToEditMap;
};

#endif // FORMDATACHART_H
