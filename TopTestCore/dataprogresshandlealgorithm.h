#ifndef DATAPROGRESSHANDLEALGORITHM_H
#define DATAPROGRESSHANDLEALGORITHM_H


#include <QVector>
#include <QPointF>
#include <QFunctionPointer>
#include <QMap>
#include <QVariant>

#include "toptestcore_global.h"


class TOPTESTCORESHARED_EXPORT DataProgressHandleAlgorithm
{
public:
    static DataProgressHandleAlgorithm* instance();

    DataProgressHandleAlgorithm();

    void setInput(const QVector<QPointF>& inputData);
    void setParameters(const QMap<QString,double>& paramterMap);
    void setAlgoType(const int& type);

    QMap<QString, QVariant> getCurrentAlgoResult();

    void update();

    QMap<QString, QVariant> getOutputData() const ;

    ///通过算法过滤温区时间区间
    QPointF parserTempSectionRange(const QList<QVector<QPointF>>& portDataList,
                                   double startTemp,double endTemp);

    int getCurrentWaveNo() const;
    void setCurrentWaveNo(int value);

    QMap<int, QPointF> getWaveToWaveStartEndMap() const;
    void setWaveToWaveStartEndMap(const QMap<int, QPointF> &value);

    void insertWaveStartEnd(const int& waveNo, const QPointF & waveStartEnd);
    QPointF getWaveStartEnd(const int& waveNo) const;


    ///TODO:for visual , cal all data to get wave data
    QList<QPointF> parserWholeWaveData(const QList<QVector<QPointF>>& inputData, const QMap<int, QPointF> &waveToWaveStartEndMap);


    ///TODO:add hand write wave index
    QMap<int, QPointF> getWaveToHandleWaveMap() const;
    void setWaveToHandleWaveMap(const QMap<int, QPointF> &value);

private:

    ///TODO:for computation
    QVector<QPointF> parserWaveData(const QVector<QPointF>& inputData, const QMap<int, QPointF> &waveToWaveStartEndMap, int currentWaveNo);

    int currentAlgorithm;

    QMap<int,QPointF> waveToWaveStartEndMap;
    int currentWaveNo = 1;

    QVector<QPointF> sourceData;
    QMap<QString,double> paramterMap;
    QMap<QString,QVariant> resultMap;

    QMap<int,QPointF> waveToHandleWaveMap;
};

#endif // DATAPROGRESSHANDLEALGORITHM_H
