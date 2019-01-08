#ifndef FORMDIALOGHEADER_H
#define FORMDIALOGHEADER_H

#include <QWidget>

namespace Ui {
class FormDialogHeader;
}

class FormDialogHeader : public QWidget
{
    Q_OBJECT

public:
    explicit FormDialogHeader(QWidget *parent = 0);
    ~FormDialogHeader();

    void setTitle(const QString& str);

signals:
    void needToCloseDialog();

private slots:
    void on_pushButton_close_clicked();

private:
    Ui::FormDialogHeader *ui;
};

#endif // FORMDIALOGHEADER_H
