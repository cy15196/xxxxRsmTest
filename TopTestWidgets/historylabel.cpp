#include "historylabel.h"
#include "ui_historylabel.h"
#include <QFile>
#include <QFileInfo>
#include <QFileIconProvider>
#include <QPainter>
#include <QDebug>
#include "clabso_ft_dpi.h"

#define TextIndent ClabsoFTDpi::convertSize(5)

struct HistoryLabelPrivate
{
    QString filePath;
    HistoryLabel *q_ptr_c;

    void refreshDisplay();
};

void HistoryLabelPrivate::refreshDisplay()
{
    if (!QFile(filePath).exists())
    {
        emit q_ptr_c->fileLose(filePath);
        filePath = "";
    }

    q_ptr_c->ui->ico->clear();
    q_ptr_c->ui->name->clear();
    q_ptr_c->ui->path->clear();

    if (!filePath.isEmpty())
    {
        QFileInfo info(filePath);

        q_ptr_c->ui->name->setText("<html><body><p style=\"text-indent: " + QString::number(TextIndent) + "px; font-weight: bold; font-size: " + QString::number(ClabsoFTDpi::convertSize(13)) + "px; color: rgb(48, 140, 198);\">" + info.completeBaseName() + "</p></body></html>");
        q_ptr_c->ui->name->setToolTip(info.completeBaseName());
        q_ptr_c->ui->path->setText("<html><body><p style=\"text-indent: " + QString::number(TextIndent) + "px;\">" + info.absoluteFilePath() + "</p></body></html>");
        q_ptr_c->ui->path->setToolTip(info.absoluteFilePath());
        q_ptr_c->ui->ico->setPixmap(QFileIconProvider().icon(info).pixmap(q_ptr_c->ui->ico->size()));
    }
}

HistoryLabel::HistoryLabel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HistoryLabel),
    d_ptr_c(new HistoryLabelPrivate)
{
    ui->setupUi(this);
    d_func_c()->q_ptr_c = this;
}

HistoryLabel::HistoryLabel(const QString &filePath, QWidget *parent) :
    HistoryLabel(parent)
{
    setFile(filePath);
}

HistoryLabel::~HistoryLabel()
{
    delete ui;
    delete d_ptr_c;
}

void HistoryLabel::setFile(const QString &filePath)
{
    d_func_c()->filePath = filePath;
    d_func_c()->refreshDisplay();
}

QString HistoryLabel::getFile() const
{
    return d_ptr_c->filePath;
}

QSize HistoryLabel::sizeHint() const
{
    return QSize(ClabsoFTDpi::convertSize(400), ClabsoFTDpi::convertSize(50));
}

void HistoryLabel::paintEvent(QPaintEvent *event)
{
    ui->ico->setMaximumHeight(ClabsoFTDpi::convertSize(32));
    ui->ico->setMinimumHeight(ClabsoFTDpi::convertSize(32));
    ui->ico->setMaximumWidth(ClabsoFTDpi::convertSize(32));
    ui->ico->setMinimumWidth(ClabsoFTDpi::convertSize(32));
    d_func_c()->refreshDisplay();
}
