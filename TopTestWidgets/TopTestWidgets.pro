#-------------------------------------------------
#
# Project created by QtCreator 2018-04-04T16:26:03
#
#-------------------------------------------------

QT       += widgets charts printsupport

TARGET = TopTestWidgets
TEMPLATE = lib

DESTDIR+=../lib
QMAKE_POST_LINK += copy $$replace(DESTDIR,/,\\)\\$$TARGET".dll" ..\bin

DEFINES += TOPTESTWIDGETS_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    formdevicewidget.cpp \
    chartwidget.cpp \
#    dialogsystemsetting.cpp \
    dialogdeviceandprogresshanding.cpp \
    dialogdevicehandle.cpp \
    dialogprogresshandle.cpp \
    formdatavisual.cpp \
    formtoptestgx80.cpp \
    formstatetime.cpp \
    formheadertable.cpp \
    formdatatable.cpp \
    dialogrsmii.cpp \
    dialoghistorylines.cpp \
    dialogdatadownload.cpp \
    formwelcome.cpp \
    historylabel.cpp \
    clabso_ft_dpi.cpp \
    formdevicewidget.cpp \
    createdialog.cpp \
    formcreatedataanalysis.cpp \
    formreport.cpp \
    formdatacurve.cpp \
    mychartview.cpp \
    formcolorlengend.cpp \
    formframelesstitlebar.cpp \
    callout.cpp \
    chartvhline.cpp \
    rulerslider.cpp \
    formdistanceindicator.cpp \
    rulerslidergraphicitem.cpp \
    formnolcd.cpp \
    formgrouplcd.cpp \
#    clabsostepaxis.cpp \
#    chartclabsostepaxisx.cpp \
#    chartclabsostepaxisy.cpp \
#    privateSource/abstractbarchartitem.cpp \
#    privateSource/abstractchartlayout.cpp \
#    privateSource/abstractdomain.cpp \
#    privateSource/areachartitem.cpp \
#    privateSource/axisanimation.cpp \
#    privateSource/bar.cpp \
#    privateSource/baranimation.cpp \
#    privateSource/barchartitem.cpp \
#    privateSource/boxplotanimation.cpp \
#    privateSource/boxplotchartitem.cpp \
#    privateSource/boxwhiskers.cpp \
#    privateSource/boxwhiskersanimation.cpp \
#    privateSource/candlestick.cpp \
#    privateSource/candlestickanimation.cpp \
#    privateSource/candlestickbodywicksanimation.cpp \
#    privateSource/candlestickchartitem.cpp \
#    privateSource/cartesianchartaxis.cpp \
#    privateSource/cartesianchartlayout.cpp \
#    privateSource/chartanimation.cpp \
#    privateSource/chartaxiselement.cpp \
#    privateSource/chartbackground.cpp \
#    privateSource/chartbarcategoryaxisx.cpp \
#    privateSource/chartbarcategoryaxisy.cpp \
#    privateSource/chartcategoryaxisx.cpp \
#    privateSource/chartcategoryaxisy.cpp \
#    privateSource/chartdataset.cpp \
#    privateSource/chartdatetimeaxisx.cpp \
#    privateSource/chartdatetimeaxisy.cpp \
#    privateSource/chartelement.cpp \
#    privateSource/chartitem.cpp \
#    privateSource/chartlogvalueaxisx.cpp \
#    privateSource/chartlogvalueaxisy.cpp \
#    privateSource/chartpresenter.cpp \
#    privateSource/chartthememanager.cpp \
#    privateSource/charttitle.cpp \
#    privateSource/chartvalueaxisx.cpp \
#    privateSource/chartvalueaxisy.cpp \
#    privateSource/glxyseriesdata.cpp \
#    privateSource/horizontalaxis.cpp \
#    privateSource/horizontalbarchartitem.cpp \
#    privateSource/horizontalpercentbarchartitem.cpp \
#    privateSource/horizontalstackedbarchartitem.cpp \
#    privateSource/legendlayout.cpp \
#    privateSource/legendmarkeritem.cpp \
#    privateSource/legendscroller.cpp \
#    privateSource/linechartitem.cpp \
#    privateSource/logxlogydomain.cpp \
#    privateSource/logxlogypolardomain.cpp \
#    privateSource/logxydomain.cpp \
#    privateSource/logxypolardomain.cpp \
#    privateSource/percentbarchartitem.cpp \
#    privateSource/pieanimation.cpp \
#    privateSource/piechartitem.cpp \
#    privateSource/piesliceanimation.cpp \
#    privateSource/piesliceitem.cpp \
#    privateSource/polarchartaxis.cpp \
#    privateSource/polarchartaxisangular.cpp \
#    privateSource/polarchartaxisradial.cpp \
#    privateSource/polarchartcategoryaxisangular.cpp \
#    privateSource/polarchartcategoryaxisradial.cpp \
#    privateSource/polarchartdatetimeaxisangular.cpp \
#    privateSource/polarchartdatetimeaxisradial.cpp \
#    privateSource/polarchartlayout.cpp \
#    privateSource/polarchartlogvalueaxisangular.cpp \
#    privateSource/polarchartlogvalueaxisradial.cpp \
#    privateSource/polarchartvalueaxisangular.cpp \
#    privateSource/polarchartvalueaxisradial.cpp \
#    privateSource/polardomain.cpp \
#    privateSource/qabstractaxis.cpp \
#    privateSource/qabstractbarseries.cpp \
#    privateSource/qabstractseries.cpp \
#    privateSource/qarealegendmarker.cpp \
#    privateSource/qareaseries.cpp \
#    privateSource/qbarcategoryaxis.cpp \
#    privateSource/qbarlegendmarker.cpp \
#    privateSource/qbarmodelmapper.cpp \
#    privateSource/qbarseries.cpp \
#    privateSource/qbarset.cpp \
#    privateSource/qboxplotlegendmarker.cpp \
#    privateSource/qboxplotmodelmapper.cpp \
#    privateSource/qboxplotseries.cpp \
#    privateSource/qboxset.cpp \
#    privateSource/qcandlesticklegendmarker.cpp \
#    privateSource/qcandlestickmodelmapper.cpp \
#    privateSource/qcandlestickseries.cpp \
#    privateSource/qcandlestickset.cpp \
#    privateSource/qcategoryaxis.cpp \
#    privateSource/qchart.cpp \
#    privateSource/qchartview.cpp \
#    privateSource/qdatetimeaxis.cpp \
#    privateSource/qhbarmodelmapper.cpp \
#    privateSource/qhboxplotmodelmapper.cpp \
#    privateSource/qhcandlestickmodelmapper.cpp \
#    privateSource/qhorizontalbarseries.cpp \
#    privateSource/qhorizontalpercentbarseries.cpp \
#    privateSource/qhorizontalstackedbarseries.cpp \
#    privateSource/qhpiemodelmapper.cpp \
#    privateSource/qhxymodelmapper.cpp \
#    privateSource/qlegend.cpp \
#    privateSource/qlegendmarker.cpp \
#    privateSource/qlineseries.cpp \
#    privateSource/qlogvalueaxis.cpp \
#    privateSource/qpercentbarseries.cpp \
#    privateSource/qpielegendmarker.cpp \
#    privateSource/qpiemodelmapper.cpp \
#    privateSource/qpieseries.cpp \
#    privateSource/qpieslice.cpp \
#    privateSource/qpolarchart.cpp \
#    privateSource/qscatterseries.cpp \
#    privateSource/qsplineseries.cpp \
#    privateSource/qstackedbarseries.cpp \
#    privateSource/qvalueaxis.cpp \
#    privateSource/qvbarmodelmapper.cpp \
#    privateSource/qvboxplotmodelmapper.cpp \
#    privateSource/qvcandlestickmodelmapper.cpp \
#    privateSource/qvpiemodelmapper.cpp \
#    privateSource/qvxymodelmapper.cpp \
#    privateSource/qxylegendmarker.cpp \
#    privateSource/qxymodelmapper.cpp \
#    privateSource/qxyseries.cpp \
#    privateSource/scatteranimation.cpp \
#    privateSource/scatterchartitem.cpp \
#    privateSource/scroller.cpp \
#    privateSource/splineanimation.cpp \
#    privateSource/splinechartitem.cpp \
#    privateSource/stackedbarchartitem.cpp \
#    privateSource/verticalaxis.cpp \
#    privateSource/xlogydomain.cpp \
#    privateSource/xlogypolardomain.cpp \
#    privateSource/xyanimation.cpp \
#    privateSource/xychart.cpp \
#    privateSource/xydomain.cpp \
#    privateSource/xypolardomain.cpp \
#    privateSource/glwidget.cpp \
    qcpchartwidget.cpp \
    qcustomplot.cpp \
    qcplabeltracer.cpp \
    lcdnumber.cpp







HEADERS +=\
        toptestwidgets_global.h \
    formdevicewidget.h \
    chartwidget.h \
#    dialogsystemsetting.h \
    dialogdeviceandprogresshanding.h \
    dialogdevicehandle.h \
    dialogprogresshandle.h \
    formdatavisual.h \
    formtoptestgx80.h \
    formstatetime.h \
    formheadertable.h \
    formdatatable.h \
    dialogrsmii.h \
    dialoghistorylines.h \
    dialogdatadownload.h \
    formwelcome.h \
    historylabel.h \
    clabso_ft_dpi.h \
    formdevicewidget.h \
    createdialog.h \
    formcreatedataanalysis.h \
    formreport.h \
    formdatacurve.h \
    mychartview.h \
    formcolorlengend.h \
    formframelesstitlebar.h \
    callout.h \
    chartvhline.h \
    rulerslider.h \
    formdistanceindicator.h \
    rulerslidergraphicitem.h \
    formnolcd.h \
    formgrouplcd.h \
#    clabsostepaxis.h \
#    private/abstractbarchartitem_p.h \
#    private/abstractchartlayout_p.h \
#    private/abstractdomain_p.h \
#    private/areachartitem_p.h \
#    private/axisanimation_p.h \
#    private/bar_p.h \
#    private/baranimation_p.h \
#    private/barchartitem_p.h \
#    private/boxplotanimation_p.h \
#    private/boxplotchartitem_p.h \
#    private/boxwhiskers_p.h \
#    private/boxwhiskersanimation_p.h \
#    private/boxwhiskersdata_p.h \
#    private/candlestick_p.h \
#    private/candlestickanimation_p.h \
#    private/candlestickbodywicksanimation_p.h \
#    private/candlestickchartitem_p.h \
#    private/candlestickdata_p.h \
#    private/cartesianchartaxis_p.h \
#    private/cartesianchartlayout_p.h \
#    private/chartanimation_p.h \
#    private/chartaxiselement_p.h \
#    private/chartbackground_p.h \
#    private/chartbarcategoryaxisx_p.h \
#    private/chartbarcategoryaxisy_p.h \
#    private/chartcategoryaxisx_p.h \
#    private/chartcategoryaxisy_p.h \
#    private/chartclabsostepaxisx_p.h \
#    private/chartclabsostepaxisy_p.h \
#    private/chartconfig_p.h \
#    private/chartdataset_p.h \
#    private/chartdatetimeaxisx_p.h \
#    private/chartdatetimeaxisy_p.h \
#    private/chartelement_p.h \
#    private/charthelpers_p.h \
#    private/chartitem_p.h \
#    private/chartlogvalueaxisx_p.h \
#    private/chartlogvalueaxisy_p.h \
#    private/chartpresenter_p.h \
#    private/charttheme_p.h \
#    private/chartthemebluecerulean_p.h \
#    private/chartthemeblueicy_p.h \
#    private/chartthemebluencs_p.h \
#    private/chartthemebrownsand_p.h \
#    private/chartthemedark_p.h \
#    private/chartthemehighcontrast_p.h \
#    private/chartthemelight_p.h \
#    private/chartthememanager_p.h \
#    private/chartthemeqt_p.h \
#    private/chartthemesystem_p.h \
#    private/charttitle_p.h \
#    private/chartvalueaxisx_p.h \
#    private/chartvalueaxisy_p.h \
#    private/clabsostepaxis_p.h \
#    private/glwidget_p.h \
#    private/glxyseriesdata_p.h \
#    private/horizontalaxis_p.h \
#    private/horizontalbarchartitem_p.h \
#    private/horizontalpercentbarchartitem_p.h \
#    private/horizontalstackedbarchartitem_p.h \
#    private/legendlayout_p.h \
#    private/legendmarkeritem_p.h \
#    private/legendscroller_p.h \
#    private/linearrowitem_p.h \
#    private/linechartitem_p.h \
#    private/logxlogydomain_p.h \
#    private/logxlogypolardomain_p.h \
#    private/logxydomain_p.h \
#    private/logxypolardomain_p.h \
#    private/percentbarchartitem_p.h \
#    private/pieanimation_p.h \
#    private/piechartitem_p.h \
#    private/piesliceanimation_p.h \
#    private/pieslicedata_p.h \
#    private/piesliceitem_p.h \
#    private/polarchartaxis_p.h \
#    private/polarchartaxisangular_p.h \
#    private/polarchartaxisradial_p.h \
#    private/polarchartcategoryaxisangular_p.h \
#    private/polarchartcategoryaxisradial_p.h \
#    private/polarchartdatetimeaxisangular_p.h \
#    private/polarchartdatetimeaxisradial_p.h \
#    private/polarchartlayout_p.h \
#    private/polarchartlogvalueaxisangular_p.h \
#    private/polarchartlogvalueaxisradial_p.h \
#    private/polarchartvalueaxisangular_p.h \
#    private/polarchartvalueaxisradial_p.h \
#    private/polardomain_p.h \
#    private/qabstractaxis_p.h \
#    private/qabstractbarseries_p.h \
#    private/qabstractseries_p.h \
#    private/qarealegendmarker_p.h \
#    private/qareaseries_p.h \
#    private/qbarcategoryaxis_p.h \
#    private/qbarlegendmarker_p.h \
#    private/qbarmodelmapper_p.h \
#    private/qbarseries_p.h \
#    private/qbarset_p.h \
#    private/qboxplotlegendmarker_p.h \
#    private/qboxplotmodelmapper_p.h \
#    private/qboxplotseries_p.h \
#    private/qboxset_p.h \
#    private/qcandlesticklegendmarker_p.h \
#    private/qcandlestickmodelmapper_p.h \
#    private/qcandlestickseries_p.h \
#    private/qcandlestickset_p.h \
#    private/qcategoryaxis_p.h \
#    private/qchart_p.h \
#    private/qchartview_p.h \
#    private/qdatetimeaxis_p.h \
#    private/qhorizontalbarseries_p.h \
#    private/qhorizontalpercentbarseries_p.h \
#    private/qhorizontalstackedbarseries_p.h \
#    private/qlegend_p.h \
#    private/qlegendmarker_p.h \
#    private/qlineseries_p.h \
#    private/qlogvalueaxis_p.h \
#    private/qpercentbarseries_p.h \
#    private/qpielegendmarker_p.h \
#    private/qpiemodelmapper_p.h \
#    private/qpieseries_p.h \
#    private/qpieslice_p.h \
#    private/qscatterseries_p.h \
#    private/qsplineseries_p.h \
#    private/qstackedbarseries_p.h \
#    private/qvalueaxis_p.h \
#    private/qxylegendmarker_p.h \
#    private/qxymodelmapper_p.h \
#    private/qxyseries_p.h \
#    private/scatteranimation_p.h \
#    private/scatterchartitem_p.h \
#    private/scroller_p.h \
#    private/splineanimation_p.h \
#    private/splinechartitem_p.h \
#    private/stackedbarchartitem_p.h \
#    private/verticalaxis_p.h \
#    private/xlogydomain_p.h \
#    private/xlogypolardomain_p.h \
#    private/xyanimation_p.h \
#    private/xychart_p.h \
#    private/xydomain_p.h \
#    private/xypolardomain_p.h \
    qcpchartwidget.h \
    qcustomplot.h \
    qcplabeltracer.h \
    lcdnumber.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    formdevicewidget.ui \
#    dialogsystemsetting.ui \
    dialogdeviceandprogresshanding.ui \
    dialogdevicehandle.ui \
    dialogprogresshandle.ui \
    formdatavisual.ui \
    formtoptestgx80.ui \
    formstatetime.ui \
    formheadertable.ui \
    formdatatable.ui \
    dialogrsmii.ui \
    dialoghistorylines.ui \
    dialogdatadownload.ui \
    formwelcome.ui \
    historylabel.ui \
    formdevicewidget.ui \
    createdialog.ui \
    formcreatedataanalysis.ui \
    formreport.ui \
    formdatacurve.ui \
    formcolorlengend.ui \
    formframelesstitlebar.ui \
    formnolcd.ui

INCLUDEPATH +=\
    ../TopTestCore\
    ../TopTestUsb\
    ../FipsItemDelegate\

LIBS += -L../lib\
    -lTopTestCore\
    -lTopTestUsb\
    -lFipsItemDelegate\
