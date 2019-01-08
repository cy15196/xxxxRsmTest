/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
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

#include <QtCharts/QChart>
#include <private/qchart_p.h>
#include <private/legendscroller_p.h>
#include <private/qlegend_p.h>
#include <private/chartbackground_p.h>
#include <QtCharts/QAbstractAxis>
#include <private/abstractchartlayout_p.h>
#include <private/charttheme_p.h>
#include <private/chartpresenter_p.h>
#include <private/chartdataset_p.h>
#include <QtWidgets/QGraphicsScene>
#include <QGraphicsSceneResizeEvent>

QT_CHARTS_BEGIN_NAMESPACE

/*!
 \enum QChart::ChartTheme

 This enum describes the theme used by the chart.

 A theme is a built-in collection of UI style related settings applied to all
 the visual elements of a chart, such as colors, pens, brushes, and fonts of
 series, as well as axes, title, and legend. The \l {Chart themes example}
 illustrates how to use themes.

 \note Changing the theme will overwrite all customizations previously applied
 to the series.

 \value ChartThemeLight
        The light theme, which is the default theme.
 \value ChartThemeBlueCerulean
        The cerulean blue theme.
 \value ChartThemeDark
        The dark theme.
 \value ChartThemeBrownSand
        The sand brown theme.
 \value ChartThemeBlueNcs
        The natural color system (NCS) blue theme.
 \value ChartThemeHighContrast
        The high contrast theme.
 \value ChartThemeBlueIcy
        The icy blue theme.
 \value ChartThemeQt
        The Qt theme.
 */

/*!
 \enum QChart::AnimationOption

 This enum describes the animations enabled in the chart.

 \value NoAnimation
        Animation is disabled in the chart. This is the default value.
 \value GridAxisAnimations
        Grid axis animation is enabled in the chart.
 \value SeriesAnimations
        Series animation is enabled in the chart.
 \value AllAnimations
        All animation types are enabled in the chart.
 */

/*!
 \enum QChart::ChartType

 This enum describes the chart type.

 \value ChartTypeUndefined
        The chart type is not defined.
 \value ChartTypeCartesian
        A cartesian chart.
 \value ChartTypePolar
        A polar chart.
 */

/*!
 \class QChart
 \inmodule Qt Charts
 \brief The QChart class manages the graphical representation of the chart's
 series, legends, and axes.

 QChart is a QGraphicsWidget that you can show in a QGraphicsScene. It manages the graphical
 representation of different types of series and other chart related objects like legend and
 axes. To simply show a chart in a layout, the convenience class QChartView can be used
 instead of QChart. In addition, line, spline, area, and scatter series can be presented as
 polar charts by using the QPolarChart class.

 \sa QChartView, QPolarChart
 */

/*!
 \property QChart::animationOptions
 \brief The animation options for the chart.

 Animations are enabled or disabled based on this setting.
 */

/*!
 \property QChart::animationDuration
 \brief The duration of the animation for the chart.
 */

/*!
 \property QChart::animationEasingCurve
 \brief The easing curve of the animation for the chart.
 */

/*!
 \property QChart::backgroundVisible
 \brief Whether the chart background is visible.
 \sa setBackgroundBrush(), setBackgroundPen(), plotAreaBackgroundVisible
 */

/*!
 \property QChart::dropShadowEnabled
 \brief Whether the background drop shadow effect is enabled.

 If set to \c true, the background drop shadow effect is enabled. If set to \c false, it
 is disabled.

 \note The drop shadow effect depends on the theme, and therefore the setting may
 change if the theme is changed.
 */

/*!
 \property QChart::backgroundRoundness
 \brief The diameter of the rounding circle at the corners of the chart background.
 */

/*!
 \property QChart::margins
 \brief The minimum margins allowed between the edge of the chart rectangle and
 the plot area.

 The margins are used for drawing the title, axes, and legend.
 */

/*!
 \property QChart::theme
 \brief The theme used for the chart.
 */

/*!
 \property QChart::title
 \brief The title of the chart.

 The title is shown as a headline on top of the chart. Chart titles support HTML formatting.
 */

/*!
 \property QChart::chartType
 \brief Whether the chart is a cartesian chart or a polar chart.

 This property is set internally and it is read only.
 \sa QPolarChart
 */

/*!
 \property QChart::plotAreaBackgroundVisible
 \brief Whether the chart plot area background is visible.

 \note By default, the plot area background is invisible and the plot area uses
 the general chart background.
 \sa setPlotAreaBackgroundBrush(), setPlotAreaBackgroundPen(), backgroundVisible
 */

/*!
  \property QChart::localizeNumbers
  \brief Whether numbers are localized.

  When \c{true}, all generated numbers appearing in various series and axis labels will be
  localized using the QLocale set with the \l locale property.
  When \c{false}, the \e C locale is always used.
  Defaults to \c{false}.

  \note This property does not affect QDateTimeAxis labels, which always use the QLocale set with
  the locale property.

  \sa locale
*/

/*!
  \property QChart::locale
  \brief The locale used to format various chart labels.

  Labels are localized only when \l localizeNumbers is \c true, except for QDateTimeAxis
  labels, which always use the QLocale set with this property.

  Defaults to the application default locale at the time when the chart is constructed.

  \sa localizeNumbers
*/

/*!
  \property QChart::plotArea
  \brief The rectangle within which the chart is drawn.

  The plot area does not include the area defined by margins.
*/

/*!
 \internal
 Constructs a chart object of \a type that is a child of \a parent.
 The properties specified by \a wFlags are passed to the QGraphicsWidget constructor.
 This constructor is called only by subclasses.
*/

/*!
 Constructs a chart object that is a child of \a parent.
 The properties specified by \a wFlags are passed to the QGraphicsWidget constructor.
 */
/*!
 Deletes the chart object and its children, such as the series and axis objects added to it.
 */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

QChartPrivate::QChartPrivate(QChart *q, QChart::ChartType type):
    q_ptr(q),
    m_legend(0),
    m_dataset(new ChartDataSet(q)),
    m_presenter(new ChartPresenter(q, type)),
    m_themeManager(new ChartThemeManager(q)),
    m_type(type)
{
    QObject::connect(m_dataset, SIGNAL(seriesAdded(QAbstractSeries*)), m_presenter, SLOT(handleSeriesAdded(QAbstractSeries*)));
    QObject::connect(m_dataset, SIGNAL(seriesRemoved(QAbstractSeries*)), m_presenter, SLOT(handleSeriesRemoved(QAbstractSeries*)));
    QObject::connect(m_dataset, SIGNAL(axisAdded(QAbstractAxis*)), m_presenter, SLOT(handleAxisAdded(QAbstractAxis*)));
    QObject::connect(m_dataset, SIGNAL(axisRemoved(QAbstractAxis*)), m_presenter, SLOT(handleAxisRemoved(QAbstractAxis*)));
    QObject::connect(m_dataset, SIGNAL(seriesAdded(QAbstractSeries*)), m_themeManager, SLOT(handleSeriesAdded(QAbstractSeries*)));
    QObject::connect(m_dataset, SIGNAL(seriesRemoved(QAbstractSeries*)), m_themeManager, SLOT(handleSeriesRemoved(QAbstractSeries*)));
    QObject::connect(m_dataset, SIGNAL(axisAdded(QAbstractAxis*)), m_themeManager, SLOT(handleAxisAdded(QAbstractAxis*)));
    QObject::connect(m_dataset, SIGNAL(axisRemoved(QAbstractAxis*)), m_themeManager, SLOT(handleAxisRemoved(QAbstractAxis*)));
    QObject::connect(m_presenter, &ChartPresenter::plotAreaChanged, q, &QChart::plotAreaChanged);
}

QChartPrivate::~QChartPrivate()
{
    delete m_themeManager;
}

// Hackish solution to the problem of explicitly assigning the default pen/brush/font
// to a series or axis and having theme override it:
// Initialize pens, brushes, and fonts to something nobody is likely to ever use,
// so that default theme initialization will always set these properly.
QPen &QChartPrivate::defaultPen()
{
    static QPen defaultPen(QColor(1, 2, 0), 0.93247536);
    return defaultPen;
}

QBrush &QChartPrivate::defaultBrush()
{
    static QBrush defaultBrush(QColor(1, 2, 0), Qt::Dense7Pattern);
    return defaultBrush;
}

QFont &QChartPrivate::defaultFont()
{
    static bool defaultFontInitialized(false);
    static QFont defaultFont;
    if (!defaultFontInitialized) {
        defaultFont.setPointSizeF(8.34563465);
        defaultFontInitialized = true;
    }
    return defaultFont;
}

void QChartPrivate::init()
{
    m_legend = new LegendScroller(q_ptr);
    q_ptr->setTheme(QChart::ChartThemeLight);
    q_ptr->setLayout(m_presenter->layout());
}

void QChartPrivate::zoomIn(qreal factor)
{
    QRectF rect = m_presenter->geometry();
    rect.setWidth(rect.width() / factor);
    rect.setHeight(rect.height() / factor);
    rect.moveCenter(m_presenter->geometry().center());
    zoomIn(rect);
}

void QChartPrivate::zoomIn(const QRectF &rect)
{
    if (!rect.isValid())
        return;

    QRectF r = rect.normalized();
    const QRectF geometry = m_presenter->geometry();
    r.translate(-geometry.topLeft());

    if (!r.isValid())
        return;

    QPointF zoomPoint(r.center().x() / geometry.width(), r.center().y() / geometry.height());
    m_presenter->setState(ChartPresenter::ZoomInState,zoomPoint);
    m_dataset->zoomInDomain(r);
    m_presenter->setState(ChartPresenter::ShowState,QPointF());

}

void QChartPrivate::zoomReset()
{
    m_dataset->zoomResetDomain();
}

bool QChartPrivate::isZoomed()
{
    return m_dataset->isZoomedDomain();
}

void QChartPrivate::zoomOut(qreal factor)
{
    const QRectF geometry = m_presenter->geometry();

    QRectF r;
    r.setSize(geometry.size() / factor);
    r.moveCenter(QPointF(geometry.size().width()/2 ,geometry.size().height()/2));
    if (!r.isValid())
        return;

    QPointF zoomPoint(r.center().x() / geometry.width(), r.center().y() / geometry.height());
    m_presenter->setState(ChartPresenter::ZoomOutState,zoomPoint);
    m_dataset->zoomOutDomain(r);
    m_presenter->setState(ChartPresenter::ShowState,QPointF());
}

void QChartPrivate::scroll(qreal dx, qreal dy)
{
    if (dx < 0) m_presenter->setState(ChartPresenter::ScrollLeftState,QPointF());
    if (dx > 0) m_presenter->setState(ChartPresenter::ScrollRightState,QPointF());
    if (dy < 0) m_presenter->setState(ChartPresenter::ScrollUpState,QPointF());
    if (dy > 0) m_presenter->setState(ChartPresenter::ScrollDownState,QPointF());

    m_dataset->scrollDomain(dx, dy);
    m_presenter->setState(ChartPresenter::ShowState,QPointF());
}

//#include "moc_qchart.cpp"

QT_CHARTS_END_NAMESPACE
