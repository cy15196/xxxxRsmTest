#pragma execution_character_set("utf-8")

#include "toptestmainwindow.h"
#include "toptestapplication.h"
#include <QSplashScreen>
#include <QMessageBox>
#include <QTextCodec>
#include <Windows.h>
#include <DbgHelp.h>
#include <QTime>
#include "mainwindow.h"
#include "rsmmainwindow.h"
#include "commonhelper.h"

#include "quiwidget.h"
#include "appinit.h"
#include "framelesshelper.h"

#include "clabso_ft_io.h"
//#include <boost/stacktrace.hpp>


#include <QApplication>
#include <iostream>
#include <cstdlib>
#include <QFile>
#include <QString>
#include <QTextStream>
#include <QMutex>
#include <QDateTime>


QMutex mutex;//日志代码互斥锁
QString timePoint;

//日志生成
void LogMsgOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
 {
     mutex.lock();
     std::cout << msg.toStdString() << std::endl;
     //Critical Resource of Code
     QByteArray localMsg = msg.toLocal8Bit();
     QString log;

     switch (type) {
     case QtDebugMsg:
         //fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
         log.append(QString("Debug  File:%1 %2  Line:%3  Content:%4").arg(context.file).arg(context.function).arg(context.line).arg(msg));
         break;
     case QtInfoMsg:
         //fprintf(stderr, "Info: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
         log.append(QString("Info: %1  %2  %3  %4").arg(localMsg.constData()).arg(context.file).arg(context.line).arg(context.function));
         break;
     case QtWarningMsg:
         //fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
         log.append(QString("Warning: %1  %2  %3  %4").arg(localMsg.constData()).arg(context.file).arg(context.line).arg(context.function));
         break;
     case QtCriticalMsg:
         //fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
         log.append(QString("Critical: %1  %2  %3  %4").arg(localMsg.constData()).arg(context.file).arg(context.line).arg(context.function));
         break;
     case QtFatalMsg:
         //fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
         log.append(QString("Fatal: %1  %2  %3  %4").arg(localMsg.constData()).arg(context.file).arg(context.line).arg(context.function));
         abort();
     }

     QFile file;
     QDir::temp().mkdir("Rsm");
     QString path = QDir::tempPath() + "/Rsm/" + QString("log%1.lgt").arg(timePoint);
     file.setFileName(path);
     if (!file.open(QIODevice::ReadWrite | QIODevice::Append))
     {
         QString erinfo = file.errorString();
         std::cout << erinfo.toStdString() << std::endl;
         return;
     }
     QTextStream out(&file);
     out << "\n\r" << log;
     file.close();

     mutex.unlock();
 }

LONG ApplicationCrashHandler(EXCEPTION_POINTERS *pException){//程式异常捕获
    /*
      ***保存数据代码***
    */
    //创建 Dump 文件
    HANDLE hDumpFile = CreateFile((LPCWSTR)QTime::currentTime().toString("hh_mm_ss_zzz.dmp").toStdWString().data(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if( hDumpFile != INVALID_HANDLE_VALUE){
        //Dump信息
        MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
        dumpInfo.ExceptionPointers = pException;
        dumpInfo.ThreadId = GetCurrentThreadId();
        dumpInfo.ClientPointers = TRUE;
        //写入Dump文件内容
        MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpNormal, &dumpInfo, NULL, NULL);
    }

//    std::stringstream ss;
//    ss << boost::stacktrace::stacktrace();
//    qDebug()<<"Current call stack:\n" << ss.str().data();
//    csLogs<<"Current call stack:\n "<<ss.str().data();

    //这里弹出一个错误对话框并退出程序
    EXCEPTION_RECORD* record = pException->ExceptionRecord;
    QString errCode(QString::number(record->ExceptionCode,16)),errAdr(QString::number((uint)record->ExceptionAddress,16)),errMod;
    QMessageBox::critical(NULL,QCoreApplication::translate("ErrorHandle","程序崩溃!"),QCoreApplication::translate("ErrorHandle","<FONT size=4><div><b>对于发生的错误，非常抱歉</b><br/></div>")+
        QCoreApplication::translate("ErrorHandle","<div>错误代码：%1</div><div>错误地址：%2</div></FONT>").arg(errCode).arg(errAdr),
        QMessageBox::Ok);
    return EXCEPTION_EXECUTE_HANDLER;
}

using namespace std;


int main(int argc, char *argv[])
{

    //release模式下，调试信息输出至日志文件
#ifndef _DEBUG
    timePoint = QDateTime::currentDateTime().toString("yyyyMMddHHmmss");
    qInstallMessageHandler(LogMsgOutput);
#endif

    TopTestApplication a(argc, argv);

    a.setOrganizationName("RSM");
    a.setApplicationName("RsmAnalysisApp");
    a.setApplicationVersion("1.0.0");

#define QUI

#ifdef QUI

    QUIHelper::setCode();
    QUIHelper::setTranslator(":/image/qt_zh_CN.qm");
    QUIHelper::setTranslator(":/image/widgets.qm");
    AppInit::Instance()->start();
#endif

    SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)ApplicationCrashHandler);//注冊异常捕获函数
    //设置启动界面
    QPixmap pixmap(":/img/splash");
    pixmap = pixmap.scaled(400,400,Qt::KeepAspectRatio);
    QSplashScreen splash(pixmap);
    //设置要显示的消息的字体属性
    splash.setFont((QFont("Helvetica", 34, QFont::Bold)));
    splash.showMessage("loading",Qt::AlignCenter,QColor::fromRgb(255,10,255));

//    不显示鼠标外观
    splash.setCursor(Qt::BlankCursor);
    splash.show();
    a.processEvents();
    splash.showMessage(a.organizationName(),Qt::AlignCenter,QColor::fromRgb(255,10,255));
    splash.showMessage(a.applicationName(),Qt::AlignCenter,QColor::fromRgb(255,10,255));

    CommonHelper::setStyle(":/qss/rsm.qss");
#ifdef QUI
    QUIWidget qui;

#if 0
    //设置无边框窗体可拉伸
    qui.setProperty("canMove", false);
    FramelessHelper *pHelper = new FramelessHelper(&qui);
    pHelper->activateOn(&qui);              //激活当前窗体
    pHelper->setTitleHeight(30);            //设置窗体的标题栏高度
    pHelper->setWidgetMovable(true);        //设置窗体可移动
    pHelper->setWidgetResizable(true);      //设置窗体可缩放
    pHelper->setRubberBandOnMove(true);     //设置橡皮筋效果-可移动
    pHelper->setRubberBandOnResize(true);   //设置橡皮筋效果-可缩放
#else
    //设置右下角可拉伸窗体
    qui.setSizeGripEnabled(true);
#endif

    //设置主窗体
    RSMMainWindow* rsm = new RSMMainWindow;
    QObject::connect(rsm,SIGNAL(windowTitleChanged(QString)),
            &qui,SLOT(setTitle(QString)));
    qui.setMainWidget(rsm);

    //设置标题
    qui.setTitle(qApp->applicationName()+"-"+qApp->applicationVersion());

    //设置标题文本居中
    qui.setAlignment(Qt::AlignVCenter);

    //设置窗体可拖动大小
    qui.setSizeGripEnabled(true);

    //设置换肤下拉菜单可见
//    qui.setVisible(QUIWidget::BtnMenu, true);

    //设置标题栏高度
//    qui.setTitleHeight(50);

    //设置按钮宽度
    //qui.setBtnWidth(50);

    //设置左上角图标-图形字体
//    qui.setIconMain(QChar(0xf072), 15);

    //设置左上角图标-图片文件
    qui.setPixmap(QUIWidget::Lab_Ico, ":/img/global");

    qui.show();
    qui.raise();

    splash.finish(&qui);
#else

    RSMMainWindow rsm;
    rsm.show();
    rsm.raise();

    splash.finish(&rsm);

#endif

//    QUIWidget *quiw = new QUIWidget;
//    quiw->setTitle("新窗体测试");
//    quiw->setAlignment(Qt::AlignCenter);
//    quiw->setMainWidget(new QWidget);
//    quiw->show();

    return a.exec();
}
