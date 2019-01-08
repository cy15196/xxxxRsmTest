#include "formdialog.h"
#include "ui_formdialog.h"

#include <QDebug>
#include <QMouseEvent>

#include "dialogbase.h"
#include "clabso_ft_dpi.h"

FormDialog::FormDialog(DialogBase *centralDialog, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormDialog),
    centralDialog(centralDialog)
{
//    this->setMinimumSize(ClabsoFTDpi::convertSize( QSize(600,400) ));
    ui->setupUi(this);
    ui->centralLayout->addWidget(centralDialog);
    this->setWindowFlags(this->windowFlags()| Qt::FramelessWindowHint);
//    this->setModal(false);
    setStanderButtons(centralDialog->standerButtons());

    connect(centralDialog,SIGNAL(windowTitleChanged(QString)),
           this,SIGNAL(windowTitleChanged(QString)));

    connect(this,SIGNAL(windowTitleChanged(QString)),
           ui->label_title,SLOT(setText(QString)));

    connect(ui->pushButton_close,SIGNAL(clicked(bool)),
            this,SLOT(close()));

    ui->pushButton_close->setIconSize(QSize(25,25));
    ui->pushButton_close->setIcon(QIcon(":/images/ic_guanbi.png"));
    this->setStyleSheet(this->styleSheet()+
                        "QWidget#widget_header{"
                        "border-style: solid;"
                        "border-width: 0px 0px 1px 0px;"
                        "border-color: #cfd7e3;"
                        "background-color: #567cdd;"
                        "}"
                        "QLabel#label_title{"
                        "color:white;"
                        "background-color: #567cdd;"
                        "}"
                        "QPushButton#pushButton_close"
                        "{"
                        "border: 2px transparent;"
                        "background-color:#567cdd;"
                        "}"
                        );

    //绑定事件过滤器监听鼠标移动
    this->installEventFilter(this);
    ui->widget_header->installEventFilter(this);
}

FormDialog::~FormDialog()
{
    delete centralDialog;
    delete ui;
}

bool FormDialog::eventFilter(QObject *obj, QEvent *evt)
{
    static QPoint mousePoint;
    static bool mousePressed = false;

    QMouseEvent *event = static_cast<QMouseEvent *>(evt);
    if (event->type() == QEvent::MouseButtonPress) {
        if (event->button() == Qt::LeftButton) {
            mousePressed = true;
            mousePoint = event->globalPos() - this->mapToGlobal(this->pos);
            return true;
        }
    } else if (event->type() == QEvent::MouseButtonRelease) {
        mousePressed = false;
        return true;
    } else if (event->type() == QEvent::MouseMove) {
        if (mousePressed && (event->buttons() && Qt::LeftButton)) {
            this->move(event->globalPos() - mousePoint);
            return true;
        }
    }

    return QWidget::eventFilter(obj, evt);
}

void FormDialog::setStanderButtons(int buttons)
{
    ui->buttonBox->setStandardButtons((QDialogButtonBox::StandardButtons)buttons);
}

//void FormDialog::mouseMoveEvent(QMouseEvent *event)
//{
//    QPoint nowPos = this->mapToParent(event->pos());
//    this->move(this->geometry().topLeft() + nowPos - pos);
//    pos = nowPos;
//    QWidget::mouseMoveEvent(event);
//}

//void FormDialog::mousePressEvent(QMouseEvent *event)
//{
//    pos = this->mapToParent(event->pos());
//    QWidget::mousePressEvent(event);
//}


void FormDialog::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Escape:
    case Qt::Key_Enter:
    case Qt::Key_Return:
        break;
    default:
        QDialog::keyPressEvent(event);
    }
}

void FormDialog::on_buttonBox_clicked(QAbstractButton *button)
{

    int standerButton = ui->buttonBox->standardButton(button);

    qDebug()<<"onButtonClicked: "<<ui->buttonBox->standardButton(button);

    centralDialog->onButtonClicked(standerButton);
}

DialogBase *FormDialog::getCentralDialog() const
{
    return centralDialog;
}

void FormDialog::setCentralDialog(DialogBase *value)
{
    centralDialog = value;
    ui->centralLayout->addWidget(centralDialog);
}
