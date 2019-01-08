#include "dialoghistorylines.h"
#include "ui_dialoghistorylines.h"

DialogHistoryLines::DialogHistoryLines(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogHistoryLines)
{
    ui->setupUi(this);
}

DialogHistoryLines::~DialogHistoryLines()
{
    delete ui;
}
