#ifndef QCPCHARTWIDGET_H
#define QCPCHARTWIDGET_H

#include <QWidget>
#include <QDateTime>
#include <QMap>
#include <QTimer>

#include "toptestwidgets_global.h"

#include "qcustomplot.h"

#include "topteststructdata.h"
#include "toptestenum.h"

class FormColorLengend;
class QCPLabelTracer;

class QCP_LIB_DECL QCPAxisTickerScaleAndOffset : public QCPAxisTicker
{
  Q_GADGET
public:
  /*!
    Defines how fractions should be displayed in tick labels.

    \see setFractionStyle
  */
//  enum FractionStyle { fsFloatingPoint     ///< Fractions are displayed as regular decimal floating point numbers, e.g. "0.25" or "0.125".
//                       ,fsAsciiFractions   ///< Fractions are written as rationals using ASCII characters only, e.g. "1/4" or "1/8"
//                       ,fsUnicodeFractions ///< Fractions are written using sub- and superscript UTF-8 digits and the fraction symbol.
//                     };
//  Q_ENUMS(FractionStyle)

  QCPAxisTickerScaleAndOffset();

  // getters:
//  QString piSymbol() const { return mPiSymbol; }
//  double piValue() const { return mPiValue; }
//  bool periodicity() const { return mPeriodicity; }
//  FractionStyle fractionStyle() const { return mFractionStyle; }

  // setters:
//  void setPiSymbol(QString symbol);
//  void setPiValue(double pi);
//  void setPeriodicity(int multiplesOfPi);
//  void setFractionStyle(FractionStyle style);

  double getScaleRatio() const;
  void setScaleRatio(double value);

  double getOffset() const;
  void setOffset(double value);

protected:
  // property members:
  //  QString mPiSymbol;
  //  double mPiValue;
//  int mPeriodicity;
//  FractionStyle mFractionStyle;
  double scaleRatio = 1.0;
  double offset = 0.0;

//  // non-property members:
//  double mPiTickStep; // size of one tick step in units of mPiValue

  // reimplemented virtual methods:
  virtual double getTickStep(const QCPRange &range) Q_DECL_OVERRIDE;
//  virtual int getSubTickCount(double tickStep) Q_DECL_OVERRIDE;
  virtual QString getTickLabel(double tick, const QLocale &locale, QChar formatChar, int precision) Q_DECL_OVERRIDE;
  virtual QVector<double> createTickVector(double tickStep, const QCPRange &range) Q_DECL_OVERRIDE;
//  // non-virtual methods:
//  void simplifyFraction(int &numerator, int &denominator) const;
//  QString fractionToString(int numerator, int denominator) const;
//  QString unicodeFraction(int numerator, int denominator) const;
//  QString unicodeSuperscript(int number) const;
//  QString unicodeSubscript(int number) const;
};

class QCP_LIB_DECL QCPAxisTickerOffsetDateTime : public QCPAxisTickerDateTime
{
  Q_GADGET
public:
  /*!
    Defines how fractions should be displayed in tick labels.

    \see setFractionStyle
  */
//  enum FractionStyle { fsFloatingPoint     ///< Fractions are displayed as regular decimal floating point numbers, e.g. "0.25" or "0.125".
//                       ,fsAsciiFractions   ///< Fractions are written as rationals using ASCII characters only, e.g. "1/4" or "1/8"
//                       ,fsUnicodeFractions ///< Fractions are written using sub- and superscript UTF-8 digits and the fraction symbol.
//                     };
//  Q_ENUMS(FractionStyle)

  QCPAxisTickerOffsetDateTime();

  // getters:
//  QString piSymbol() const { return mPiSymbol; }
//  double piValue() const { return mPiValue; }
//  bool periodicity() const { return mPeriodicity; }
//  FractionStyle fractionStyle() const { return mFractionStyle; }

  // setters:
//  void setPiSymbol(QString symbol);
//  void setPiValue(double pi);
//  void setPeriodicity(int multiplesOfPi);
//  void setFractionStyle(FractionStyle style);

  void setOffset(const QDateTime& dateTime);

  qint64 getOffset() const;
  void setOffset(const qint64& value);

protected:
  // property members:
  //  QString mPiSymbol;
  //  double mPiValue;
//  int mPeriodicity;
//  FractionStyle mFractionStyle;
  qint64 offset = 0;

//  // non-property members:
//  double mPiTickStep; // size of one tick step in units of mPiValue

  // reimplemented virtual methods:
//  virtual double getTickStep(const QCPRange &range) Q_DECL_OVERRIDE;
//  virtual int getSubTickCount(double tickStep) Q_DECL_OVERRIDE;
  virtual QString getTickLabel(double tick, const QLocale &locale, QChar formatChar, int precision) Q_DECL_OVERRIDE;
//  virtual QVector<double> createTickVector(double tickStep, const QCPRange &range) Q_DECL_OVERRIDE;
//  // non-virtual methods:
//  void simplifyFraction(int &numerator, int &denominator) const;
//  QString fractionToString(int numerator, int denominator) const;
//  QString unicodeFraction(int numerator, int denominator) const;
//  QString unicodeSuperscript(int number) const;
//  QString unicodeSubscript(int number) const;
};

class QCP_LIB_DECL QCPGraphWave : public QCPGraph
{
  Q_OBJECT
  /// \cond INCLUDE_QPROPERTIES
//  Q_PROPERTY(LineStyle lineStyle READ lineStyle WRITE setLineStyle)
//  Q_PROPERTY(QCPScatterStyle scatterStyle READ scatterStyle WRITE setScatterStyle)
//  Q_PROPERTY(int scatterSkip READ scatterSkip WRITE setScatterSkip)
//  Q_PROPERTY(QCPGraph* channelFillGraph READ channelFillGraph WRITE setChannelFillGraph)
//  Q_PROPERTY(bool adaptiveSampling READ adaptiveSampling WRITE setAdaptiveSampling)
  /// \endcond
public:
  /*!
    Defines how the graph's line is represented visually in the plot. The line is drawn with the
    current pen of the graph (\ref setPen).
    \see setLineStyle
  */
//  enum LineStyle { lsNone        ///< data points are not connected with any lines (e.g. data only represented
//                                 ///< with symbols according to the scatter style, see \ref setScatterStyle)
//                   ,lsLine       ///< data points are connected by a straight line
//                   ,lsStepLeft   ///< line is drawn as steps where the step height is the value of the left data point
//                   ,lsStepRight  ///< line is drawn as steps where the step height is the value of the right data point
//                   ,lsStepCenter ///< line is drawn as steps where the step is in between two data points
//                   ,lsImpulse    ///< each data point is represented by a line parallel to the value axis, which reaches from the data point to the zero-value-line
//                 };
//  Q_ENUMS(LineStyle)

  explicit QCPGraphWave(QCPAxis *keyAxis, QCPAxis *valueAxis);
  virtual ~QCPGraphWave();

  // getters:
//  QSharedPointer<QCPGraphDataContainer> data() const { return mDataContainer; }
//  LineStyle lineStyle() const { return mLineStyle; }
//  QCPScatterStyle scatterStyle() const { return mScatterStyle; }
//  int scatterSkip() const { return mScatterSkip; }
//  QCPGraph *channelFillGraph() const { return mChannelFillGraph.data(); }
//  bool adaptiveSampling() const { return mAdaptiveSampling; }

  // setters:
//  void setData(QSharedPointer<QCPGraphDataContainer> data);
//  void setData(const QVector<double> &keys, const QVector<double> &values, bool alreadySorted=false);
//  void setLineStyle(LineStyle ls);
//  void setScatterStyle(const QCPScatterStyle &style);
//  void setScatterSkip(int skip);
//  void setChannelFillGraph(QCPGraph *targetGraph);
//  void setAdaptiveSampling(bool enabled);

  // non-property methods:
//  void addData(const QVector<double> &keys, const QVector<double> &values, bool alreadySorted=false);
//  void addData(double key, double value);

  // reimplemented virtual methods:
//  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const Q_DECL_OVERRIDE;
//  virtual QCPRange getKeyRange(bool &foundRange, QCP::SignDomain inSignDomain=QCP::sdBoth) const Q_DECL_OVERRIDE;
//  virtual QCPRange getValueRange(bool &foundRange, QCP::SignDomain inSignDomain=QCP::sdBoth, const QCPRange &inKeyRange=QCPRange()) const Q_DECL_OVERRIDE;

    void getLinesWithOutgetOptimized(QVector<QPointF> *lines, const QCPDataRange &dataRange) const;
    QList<QPointF> getWaveRangeList() const;
    void setWaveRangeList(const QList<QPointF> &value);

    QList<QCPDataRange> getSaveWaveSegments() const;
    void setSaveWaveSegments(const QList<QCPDataRange> &value);

    bool getInWaveMode() const;
    void setInWaveMode(bool value);

protected:
    // property members:

    bool inWaveMode = true;

    QList<QPointF> waveRangeList;
    QList<QCPDataRange> saveWaveSegments;
    //  LineStyle mLineStyle;
    //  QCPScatterStyle mScatterStyle;
//  int mScatterSkip;
//  QPointer<QCPGraph> mChannelFillGraph;
//  bool mAdaptiveSampling;

  // reimplemented virtual methods:
  virtual void draw(QCPPainter *painter) Q_DECL_OVERRIDE;
//  virtual void drawLegendIcon(QCPPainter *painter, const QRectF &rect) const Q_DECL_OVERRIDE;

//  // introduced virtual methods:
    virtual void getWaveSegments(QList<QCPDataRange> & waveSegments);
//  virtual void drawFill(QCPPainter *painter, QVector<QPointF> *lines) const;
//  virtual void drawScatterPlot(QCPPainter *painter, const QVector<QPointF> &scatters, const QCPScatterStyle &style) const;
//  virtual void drawLinePlot(QCPPainter *painter, const QVector<QPointF> &lines) const;
//  virtual void drawImpulsePlot(QCPPainter *painter, const QVector<QPointF> &lines) const;

//  virtual void getOptimizedLineData(QVector<QCPGraphData> *lineData, const QCPGraphDataContainer::const_iterator &begin, const QCPGraphDataContainer::const_iterator &end) const;
//  virtual void getOptimizedScatterData(QVector<QCPGraphData> *scatterData, QCPGraphDataContainer::const_iterator begin, QCPGraphDataContainer::const_iterator end) const;

//  // non-virtual methods:
//  void getVisibleDataBounds(QCPGraphDataContainer::const_iterator &begin, QCPGraphDataContainer::const_iterator &end, const QCPDataRange &rangeRestriction) const;
//  void getLines(QVector<QPointF> *lines, const QCPDataRange &dataRange) const;
//  void getScatters(QVector<QPointF> *scatters, const QCPDataRange &dataRange) const;
//  QVector<QPointF> dataToLines(const QVector<QCPGraphData> &data) const;
//  QVector<QPointF> dataToStepLeftLines(const QVector<QCPGraphData> &data) const;
//  QVector<QPointF> dataToStepRightLines(const QVector<QCPGraphData> &data) const;
//  QVector<QPointF> dataToStepCenterLines(const QVector<QCPGraphData> &data) const;
//  QVector<QPointF> dataToImpulseLines(const QVector<QCPGraphData> &data) const;
//  QVector<QCPDataRange> getNonNanSegments(const QVector<QPointF> *lineData, Qt::Orientation keyOrientation) const;
//  QVector<QPair<QCPDataRange, QCPDataRange> > getOverlappingSegments(QVector<QCPDataRange> thisSegments, const QVector<QPointF> *thisData, QVector<QCPDataRange> otherSegments, const QVector<QPointF> *otherData) const;
//  bool segmentsIntersect(double aLower, double aUpper, double bLower, double bUpper, int &bPrecedence) const;
//  QPointF getFillBasePoint(QPointF matchingDataPoint) const;
//  const QPolygonF getFillPolygon(const QVector<QPointF> *lineData, QCPDataRange segment) const;
//  const QPolygonF getChannelFillPolygon(const QVector<QPointF> *lineData, QCPDataRange thisSegment, const QVector<QPointF> *otherData, QCPDataRange otherSegment) const;
//  int findIndexBelowX(const QVector<QPointF> *data, double x) const;
//  int findIndexAboveX(const QVector<QPointF> *data, double x) const;
//  int findIndexBelowY(const QVector<QPointF> *data, double y) const;
//  int findIndexAboveY(const QVector<QPointF> *data, double y) const;
//  double pointDistance(const QPointF &pixelPoint, QCPGraphDataContainer::const_iterator &closestData) const;

//  friend class QCustomPlot;
//  friend class QCPLegend;
  friend class QCPChartWidget;
};

class TOPTESTWIDGETSSHARED_EXPORT QCPChartWidget:public QWidget
{
    Q_OBJECT
public:

    FT_stringEnum(TimeLabelType,
        TimeLabel_S,
        TimeLabel_MS,
        TimeLabel_HMS,
        TimeLabel_Time,
        TimeLabel_DateTime)

    FT_stringEnum(TemperatureLabelType,
        TemperatureLabel_C,
        TemperatureLabel_F,
        TemperatureLabel_K)

    FT_stringEnum(DistanceLabelType,
        DistanceLabel_M,
        DistanceLabel_Cm,
        DistanceLabel_Inch,
        DistanceLabel_Feet)

    QCPChartWidget(QWidget*parent = Q_NULLPTR);
    ~QCPChartWidget();

    //init
    void reinit();


    //handle data
    QVector<QPointF> getPortPointFVector(int portNo);
    QList<QVector<QPointF> > getAllPortPointFVector();

    ///获取当前竖线所处位置对应的各通道数据
    QList<double> getCurrentPortsTempList(double currentTime);

    void setChartData(const QByteArray& byteArray);
    void clearChart();

    void setPortNum(const int & portNum);
    void setStartDateTime(const QDateTime& testDateTime );
    void setSampleTimeInteral(const double & sampleInteral);

    int getPortNum() const;
    double getSampleInteral() const;
    QDateTime getStartDateTime() const;

    //handle axis



    QMap<QString,TopTest::AxisLabelInfo> getAxisLabelInfos();


    //handle lengend
    FormColorLengend *getColorLengend() const;
    void setColorLengend(FormColorLengend *value);

    QMap<int,QPair<QString,QColor>> getPortInfoMap();
    void setPortInfos(const QMap<int,QPair<QString,QColor>>& portInfoMap);

    int getTimeType() const;
    void setTimeType(const TimeLabelType &value);

    double getVelocity() const;
    void setVelocity(double value);

    void changeToolTipEnable();

    void changeTimeFitType();

    ///绘制温区线
    void reDrawTempSection(const TopTest::TempSection& tempSection);
    void changeTempSectionVisiable();

    QString getSourceDataString();

    QByteArray getCurveData() const;

    bool getInFullRange() const;
    void setInFullRange(bool value);

//    void drawWave(const QMap<int,QList<QPointF>>& portWaveEndPointListMap);

    //time axis static functions
    static QString getTimeValueToTypeLabel(double value, int timeType);


    QList<QPointF> getWaveRangeList() const;
    void appendWaveDef(const QPointF& value);
    void setWaveRangeList(const QList<QPointF> &value);

    //get port wave data
    QVector<QPointF> getPortWaveData(int portNo, int waveNo);

    bool getDrawWave() const;
    void setDrawWave(bool value);

signals:
    void timeTypeChanged(int type);
    void tempTypeChanged(int type);
    void currentTimeChanged(double);

public slots:
    void addOneSampleData(const QByteArray& onePieceData,
                          const double& time );

    void addOneSampleData(const QList<qreal>& portDataList,
                          const double& time );

    void changeTimeLabelType(int type);
    void changeTempLabelType(int type);
    void changeDistanceLabelType(int type);

    void replot();

    void moveToTime(double time);

    void turnAutoScrollOn();

    void setAxisLabelInfo(const QMap<QString,TopTest::AxisLabelInfo>& info);

    void addLine(const TopTest::LineInfo& line);
    void clearAllLine();

    void reFit();

    void setWaveInfo(const TopTest::WaveInfo& waveInfo);
    TopTest::WaveInfo getWaveInfo();

    void applyHandleTimeSectionToLines(const TopTest::WaveInfo &waveInfo);


private slots:
    void onMouseMove(QMouseEvent* event);

    void onTimeRangeChanged(QCPRange range);

    void onAxisDoubleClick(QCPAxis* axis,QCPAxis::SelectablePart parts,QMouseEvent* event);

    void onLineVisibleChanged(QCPGraph* line, bool vi);

private:

    bool drawWave = false;

    int timeType = TimeLabel_S;
    int tempType = TemperatureLabel_C;
    int disType = DistanceLabel_M;

    QList<QPointF> waveRangeList;

    QCustomPlot* customPlot;
    QCPItemStraightLine* HLine;
    QCPItemStraightLine* VLine;
    QCPItemEllipse* Pos;
    QCPItemTracer *mCenterTracer;
    QCPLabelTracer* xAxisTracer;
    QCPLabelTracer* yAxisTracer;
    QCPLabelTracer* rAxisTracer;
//    QCPLabelTracer* xLabelUnit;
//    QCPLabelTracer* yLabelUnit;
//    QCPLabelTracer* rLabelUnit;


    QMap<int,QCPGraphWave*> portToGraphMap;
    QMap<QCPGraph*,QList<QCPGraph*>> lineToWaveListMap;
//    QMap<QCPGraphWave*, > graphToLengendItemMap;
    FormColorLengend * colorLengend;
//    RulerSlider* rs;

    int portNum = 0;
    QDateTime startDateTime = QDateTime::currentDateTime();
    double sampleInteral = 0.0;
    double tempTime = 0.0;

    double velocity = 0.3;

    bool showTempSection = false;
    bool ToolTipEnable = false;
    bool autoScroll = true;
    QList<QCPAbstractItem*> m_tempSectionSeries;

    QList<QCPAbstractItem*> m_HVLines;

    QList<QCPAbstractItem*> m_waveSections;

    QTimer autoScrollTimer;

    QByteArray curveData;

    bool inFullRange = true;
    double size =  30;

    TopTest::WaveInfo waveinfo;

};

#endif // QCPCHARTWIDGET_H
