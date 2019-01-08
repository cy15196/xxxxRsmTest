#ifndef FORMDATAVISUAL_H
#define FORMDATAVISUAL_H

#include <QWidget>
#include "toptestwidgets_global.h"

namespace Ui {
class FormDataVisual;
}

class TOPTESTWIDGETSSHARED_EXPORT FormDataVisual : public QWidget
{
    Q_OBJECT

public:
    explicit FormDataVisual(QWidget *parent = 0);
    static FormDataVisual* lastInstance();
    ~FormDataVisual();

    bool openDataFile(const QString & dataFile);
    ///set active device and process
    void setCurrentDevice(const QString& name);
    void setCurrentProcess(const QString& name);


signals:
    void gotoReport();

private slots:
    void on_toolButton_DataDownload_clicked();

    void on_toolButton_Report_clicked();

private:
    Ui::FormDataVisual *ui;
};

#endif // FORMDATAVISUAL_H
