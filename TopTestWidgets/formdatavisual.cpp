#include "formdatavisual.h"
#include "ui_formdatavisual.h"

#include "dialogdatadownload.h"
#include <QDebug>
#include "formdatacurve.h"
FormDataVisual* formDataVisual = 0;
FormDataVisual::FormDataVisual(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormDataVisual)
{
    ui->setupUi(this);
    ui->mdiArea->addSubWindow(new FormDataCurve(this));
    formDataVisual = this;
}

FormDataVisual *FormDataVisual::lastInstance()
{
    return formDataVisual;
}

FormDataVisual::~FormDataVisual()
{
    delete ui;
}

bool FormDataVisual::openDataFile(const QString &dataFile)
{
    return true;
}

void FormDataVisual::setCurrentDevice(const QString &name)
{

}

void FormDataVisual::setCurrentProcess(const QString &name)
{

}

void FormDataVisual::on_toolButton_DataDownload_clicked()
{
    DialogDataDownload ddd;
    ddd.exec();
}

void FormDataVisual::on_toolButton_Report_clicked()
{
    qDebug()<<"go to report";
    emit this->gotoReport();
}
