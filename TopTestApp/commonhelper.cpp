#include "commonhelper.h"


#include <QFile>
#include <QApplication>

void CommonHelper::setStyle(const QString &style) {
    QFile qss(style);
    qss.open(QFile::ReadOnly);
    qApp->setStyleSheet(qss.readAll());
    qss.close();
}
