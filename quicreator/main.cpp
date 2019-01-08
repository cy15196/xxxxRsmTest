#include "quiwidget.h"
#include "quicreator.h"
#include "appinit.h"
#include "framelesshelper.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setFont(QFont("Microsoft Yahei", 9));
    a.setWindowIcon(QIcon(":/main.ico"));

    QUIHelper::setCode();
    QUIHelper::setTranslator(":/image/qt_zh_CN.qm");
    QUIHelper::setTranslator(":/image/widgets.qm");
    AppInit::Instance()->start();

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
    QUICreator *creator = new QUICreator;
    qui.setMainWidget(creator);
    QObject::connect(&qui, SIGNAL(changeStyle(QString)), creator, SLOT(changeStyle(QString)));

    //设置标题
#ifdef demo
    qui.setTitle("QUI皮肤生成器 演示版 (作者: 飞扬青云  QQ: 517216493)");
#else
    qui.setTitle("QUI皮肤生成器 (作者: 飞扬青云  QQ: 517216493)");
#endif

    //设置标题文本居中
    qui.setAlignment(Qt::AlignCenter);

    //设置窗体可拖动大小
    qui.setSizeGripEnabled(true);

    //设置换肤下拉菜单可见
    qui.setVisible(QUIWidget::BtnMenu, true);

    //设置标题栏高度
    //qui.setTitleHeight(50);

    //设置按钮宽度
    //qui.setBtnWidth(50);

    //设置左上角图标-图形字体
    qui.setIconMain(QChar(0xf072), 15);

    //设置左上角图标-图片文件
    //qui.setPixmap(QUIWidget::Lab_Ico, ":/main.ico");

    qui.show();

    return a.exec();
}
