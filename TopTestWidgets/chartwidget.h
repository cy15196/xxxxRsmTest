/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Charts module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QtCharts/QChartGlobal>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtWidgets/QWidget>
#include <QtWidgets/QGraphicsWidget>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGraphicsGridLayout>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGroupBox>
#include <QtCharts/QLineSeries>
//#include "toptestwidgets_global.h"
#include "toptestwidgets_global.h"

#include "topteststructdata.h"

class FormColorLengend;
class MyChartView;

QT_CHARTS_USE_NAMESPACE

class TOPTESTWIDGETSSHARED_EXPORT ChartWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ChartWidget(QWidget *parent = 0);

    int getPortNum() const;
    void setPortNum(int value);

    double getSampleTime() const;
    void setSampleTime(double value);

    FormColorLengend *getColorLengend() const;
    void setColorLengend(FormColorLengend *value);

    QVector<QPointF> getPortPointFVector(int portNo);

    QList<QVector<QPointF> > getAllPortPointFVector();

    QMap<int,QPair<QString,QColor>> getPortInfoMap();
    void setPortInfos(const QMap<int,QPair<QString,QColor>>& portInfoMap);

    QMap<QString,TopTest::AxisLabelInfo> getAxisLabelInfos();

    bool getToolTipEnable() const;

    //return data in Data,Time,port0value,...,portNValue
    QString getSourceDataString();


    QDateTime getTestDateTime() const;
    void setTestDateTime(const QDateTime &value);

    void moveSliderToDistance(const double& distance);
    void setSliderRange(const double&max,const double& min);

    ///获取当前竖线所处位置对应的各通道数据
    QList<double> getCurrentPortsTempList(double currentTime);


//    QByteArray saveChartToCurveData();
    void clearData();

    QByteArray getCurveData() const;

    void hideForGrab(bool hide);

signals:
    void currentTimeChanged(double time);

public slots:

    void setToolTipEnable(bool value);
    void setPortLineSeries(const int& portNum,QLineSeries * series);
    void addPortTimeData(const int& portNum,const QList<QPointF>& data);

//    void addTimeData(const QMap<int,QList<QPointF> >& portDataMap);

    void addSeries();
    void removeSeries();
    void connectMarkers();
    void disconnectMarkers();

    void handleMarkerClicked();

    void reset();

    void loadDataFile(const QString& dataFile);

    void setCurveData(const QByteArray& dataArray);
    void addCurveData(const double& sampleTime,const QByteArray& dataArray);

    void addOneSampleData(const double& sampleTime,
                          const QList<double>& portDataList);

    void onToolTipEnableChanged(bool enable);

    void changeToDrawHLineMode();
    void changeToDrawVLineMode();


    //functions to add line with given value
    void addHLine(double tempValue);
    void addVLine(double timeValue);

    void setAxisLabelInfo(const QMap<QString, TopTest::AxisLabelInfo> &infos);
    void addLine(const TopTest::LineInfo& lineInfo);


    void clearAllLine();

    ///绘制温区线
    void reDrawTempSection(const TopTest::TempSection& tempSection);

    void changeTempSectionVisiable();

private:

    int portNum;
    double sampleTime;

    QChart *m_chart;
    QList<QLineSeries *> m_series;
    QPointF timeRange;
    QPointF dataRange;

    QDateTime testDateTime;

    QMap<int,QLineSeries*> portSeriresMap;

    MyChartView *m_chartView;
    QGridLayout *m_mainLayout;
    QGridLayout *m_fontLayout;

    FormColorLengend* colorLengend;

    bool toolTipEnable;

    QByteArray curveData;

    bool showTempSection;
    QList<QLineSeries *> m_tempSectionSeries;
};

#endif // MAINWIDGET_H
