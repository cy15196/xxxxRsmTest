#include "rsmdialogbase.h"
#include "ui_rsmdialogbase.h"

RsmDialogBase::RsmDialogBase(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RsmDialogBase)
{
    ui->setupUi(this);
}

RsmDialogBase::~RsmDialogBase()
{
    delete ui;
}
