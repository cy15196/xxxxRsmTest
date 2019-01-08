#ifndef FORMLINEMANAGER_H
#define FORMLINEMANAGER_H

#include <QWidget>
#include "dialogbase.h"

namespace Ui {
class FormLineManager;
}

namespace QtCharts {
class QLineSeries;
}

namespace TopTest
{
struct LineInfo;
}
class FormLineManager : public DialogBase
{
    Q_OBJECT

public:
    explicit FormLineManager(QWidget *parent = 0);
    ~FormLineManager();

    int standerButtons();
    void onButtonClicked(int);

    void setCurrentLines(const QList<QtCharts::QLineSeries*>& lineList);
    void updateCurrentTable();

signals:
    void needToAddLine(const TopTest::LineInfo& lineInfo);
    void needToRemoveLine(const QList<QtCharts::QLineSeries*>& lineList);
    void needToHighlightLine(const QList<QtCharts::QLineSeries*>& lineList);

    void needToClearAll();


private slots:
    void on_toolButton_add_clicked();
    void on_comboBox_lineType_currentIndexChanged(int index);


    void on_toolButton_clear_clicked();

private:

    void init();

    QList<QtCharts::QLineSeries*> currentLineList;
    Ui::FormLineManager *ui;
};

#endif // FORMLINEMANAGER_H
