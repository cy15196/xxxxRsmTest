#include "dialogbase.h"

DialogBase::DialogBase(QWidget *parent)
    :QWidget(parent)
{

}

DialogBase::~DialogBase()
{

}

int DialogBase::standerButtons()
{
    return QDialogButtonBox::Ok | QDialogButtonBox::Cancel;
}
