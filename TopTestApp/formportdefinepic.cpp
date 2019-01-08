#include "formportdefinepic.h"
#include "ui_formportdefinepic.h"

#include "formdraglabel.h"
#include <QFileDialog>

FormPortDefinePic::FormPortDefinePic(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormPortDefinePic)
{
    ui->setupUi(this);

    ui->label_device_pic->setPixmap(
                QPixmap(":/images/ic_shangchauntupianda.png")
                .scaledToHeight(ui->label_device_pic->height()));


//    ui->label->stackUnder(ui->pushButton);


    for(int i=0;i<16;i++)
    {
        FormDragLabel* label = new FormDragLabel(this);
        label->setObjectName(QString("form_drag_label_%1").arg(i+1));
        label->setPortNo(i+1);
        label->setPortColor(QColor::colorNames().at(i));
        label->setPos(QPoint(20 + i%8 * 25,
                    20 + i/8 * 25));
        connect(label,SIGNAL(dragPosChanged()),
                this,SIGNAL(contentChanged()));
    }

}

FormPortDefinePic::~FormPortDefinePic()
{
    delete ui;
}

void FormPortDefinePic::on_pushButton_clicked()
{
    QString pictureFilePath = QFileDialog::getOpenFileName(
                this,"Choose Picture File",
                QDir::homePath()+"/Pictures",
                "Picture files (*.png *.jpg *.jpeg)");
    if(!pictureFilePath.isEmpty())
    {
        ui->label_device_pic->setPixmap(
                    QPixmap(pictureFilePath).scaledToHeight(ui->label_device_pic->height()));

        emit this->contentChanged();

    }
}

void FormPortDefinePic::clearContent()
{
    ui->label_device_pic->setPixmap(
                QPixmap(":/images/ic_shangchauntupianda.png").scaledToHeight(ui->label_device_pic->height()));

    foreach (FormDragLabel*label, this->findChildren<FormDragLabel*>())
    {
        int portNo = QString(label->objectName().split("_").last()).toInt() - 1;
        label->setPortColor(QColor::colorNames().at(portNo));
        label->setPos(QPoint(20 + portNo%8 * 25,
                    20 + portNo/8 * 25));
    }

}

void FormPortDefinePic::setPushButtonPos(const QPoint &pos)
{
    ui->pushButton->move(pos);
}
