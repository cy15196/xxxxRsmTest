#ifndef FORMREPORT_H
#define FORMREPORT_H

#include <QWidget>
#include "toptestwidgets_global.h"

namespace Ui {
class FormReport;
}

class QPrinter;

class TOPTESTWIDGETSSHARED_EXPORT FormReport : public QWidget
{
    Q_OBJECT

public:
    explicit FormReport(QWidget *parent = 0);
    ~FormReport();

signals:
    void needToShow();

private slots:
    void on_toolButton_print_clicked();

private slots:
    void doPrint();
    void doPrintPreview();
    void printPreview(QPrinter *printer);
    void createPdf();
    void setUpPage();


private:
    Ui::FormReport *ui;
};

#endif // FORMREPORT_H
