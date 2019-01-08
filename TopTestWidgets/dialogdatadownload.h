#ifndef DIALOGDATADOWNLOAD_H
#define DIALOGDATADOWNLOAD_H

#include <QDialog>

namespace Ui {
class DialogDataDownload;
}

class DialogDataDownload : public QDialog
{
    Q_OBJECT

public:
    explicit DialogDataDownload(QWidget *parent = 0);
    ~DialogDataDownload();

    void refreshData();

private:

    void accept();

    Ui::DialogDataDownload *ui;
};

#endif // DIALOGDATADOWNLOAD_H
