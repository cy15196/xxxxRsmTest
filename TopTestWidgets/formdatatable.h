#ifndef FORMDATATABLE_H
#define FORMDATATABLE_H

#include <QWidget>
#include <QMap>
#include <QVariant>
#include "toptestwidgets_global.h"

class QTableWidget;
class QTableWidgetItem;

namespace Ui {
class FormDataTable;
}

class TOPTESTWIDGETSSHARED_EXPORT FormDataTable : public QWidget
{
    Q_OBJECT

public:
    explicit FormDataTable(QWidget *parent = 0);
    ~FormDataTable();

public slots:
    void changeToType(int type);
    void loadTypeData(int type);
    void saveTypeData();

    ///on add and remove button
    void addData();
    void removeData();

    QMap<QString,QVariant> saveTableDataToMap();
    void loadMapToTable(const QMap<QString,QVariant>& data);


    void clear();

    QString getPortNumString(int portNum);

signals:
    void changedToType(int type);

    void tableEdited(int type);

    void dataSaved();

private slots:
    void onDataTableWidgetChanged(QTableWidgetItem* item);

private:

    void initTableWidget(QTableWidget* tableWidget, int type);

    void resizeEvent(QResizeEvent* event);
    Ui::FormDataTable *ui;

    int portNum = 8;

    QTableWidget* currentTable;
    QMap<int,QTableWidget*> typeTableMap;
};

#endif // FORMDATATABLE_H
