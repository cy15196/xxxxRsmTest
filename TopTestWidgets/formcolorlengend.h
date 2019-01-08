#ifndef FORMCOLORLENGEND_H
#define FORMCOLORLENGEND_H

#include <QWidget>
#include <QMap>
#include "toptestwidgets_global.h"

namespace Ui {
class FormColorLengend;
}

class QToolButton;

class QCPGraph;

class QTableWidgetItem;

class TOPTESTWIDGETSSHARED_EXPORT FormColorLengend : public QWidget
{
    Q_OBJECT

public:
    explicit FormColorLengend(QWidget *parent = 0);
    ~FormColorLengend();

    void setPortNum(const int& portNum);
    inline void setPortName(const int& portNo, const QString& name);
    inline void setPortColor(const int& portNo, const QString& colorName);

    QMap<int,QPair<QString,QColor>> getPortInfoMap();
    void setPortInfos(const QMap<int,QPair<QString,QColor>>& portInfoMap);

    void resetPortToNum(const int &portNum, bool reset = false);
    void setPortVisualObject(const int &portNo, QCPGraph *line);

    int getLineNum() const;
    void setLineNum(int value);

    void clearData();

signals:
    void lineNumChanged(const int& num);
    void lineVisibleChanged(QCPGraph* line,bool vi);
    void needReplot();

private slots:
    void onButtonCheckStateChanged(bool checked);

    void on_tableWidget_currentItemChanged(QTableWidgetItem *current, QTableWidgetItem *previous);

    void onLineNumChanged(const int& num );

private:

    void initTable();

    int lineNum;

    Ui::FormColorLengend *ui;
    QMap<QToolButton*, QCPGraph*> buttonToLine;

};

#endif // FORMCOLORLENGEND_H
