#ifndef FORMRESULTTABLE_H
#define FORMRESULTTABLE_H

#include <QWidget>
#include <QMap>

namespace Ui {
class FormResultTable;
}

class QTableWidget;

class FormResultTable : public QWidget
{
    Q_OBJECT

public:
    explicit FormResultTable(QWidget *parent = 0);
    ~FormResultTable();

    void setPortInfo(const QMap<int,QPair<QString,QColor>> & portInfo);

    ///clear all data and init port names
    void removeAllColumn(int remianNum = 0);

    ///fill in main result from apply progress
    void fillResult(const QMap<int,QList<QMap<int,QVariant>>>& progressResult );

    void test();


    QTableWidget* getTableWidget();

private slots:
    void onTimeTypeChanged(int timeType);
    void onTempTypeChanged(int tempType);

private:

    QString convertTimeValue(const QString& currentStr, int toType);

    QMap<int,QPair<QString,QColor>> currentPortInfo;
    QMap<int,QList<QMap<int,QVariant>>> currentProgressResult;

    Ui::FormResultTable *ui;

    int currentTimeType = 0;
    int currentTempType = 0;
};

#endif // FORMRESULTTABLE_H
