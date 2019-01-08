#ifndef DIALOGOPENFILE_H
#define DIALOGOPENFILE_H

#include <QDialog>

#include "dialogbase.h"

namespace Ui {
class DialogOpenFile;
}

class QTableWidgetItem;

class DialogOpenFile : public DialogBase
{
    Q_OBJECT

public:
    explicit DialogOpenFile(QWidget *parent = 0);
    ~DialogOpenFile();

    int standerButtons() override;
    void onButtonClicked(int button) override;

signals:
    void needToOpenFile(const QString& filePath);

private slots:
    void on_listWidget_product_currentRowChanged(int currentRow);
    void record_sortbyclounm(int column);

    void on_tableWidget_files_itemDoubleClicked(QTableWidgetItem *item);

    void on_pushButton_DirectOpen_clicked();

private:

    void init();

    QString workDir;

    Ui::DialogOpenFile *ui;
};

#endif // DIALOGOPENFILE_H
