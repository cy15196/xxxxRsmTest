#include "formdatacurve.h"
#include "ui_formdatacurve.h"

FormDataCurve::FormDataCurve(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormDataCurve)
{
    ui->setupUi(this);
}

FormDataCurve::~FormDataCurve()
{
    delete ui;
}
