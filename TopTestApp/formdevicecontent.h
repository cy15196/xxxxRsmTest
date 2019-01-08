#ifndef FORMDEVICECONTENT_H
#define FORMDEVICECONTENT_H

#include "formfilesettingbasewidget.h"


namespace Ui {
class FormDeviceContent;
}

class QTableWidgetItem;

class FormDeviceContent : public FileSettingWidget
{
    Q_OBJECT

public:
    explicit FormDeviceContent(QWidget *parent = 0);
    ~FormDeviceContent();

    // FileSettingWidget interface
    QString getFileTypeDisplayText();
    QString getFileTypeSettingKeyName();
    void loadSettigRecord(const QVariant&  recordName);
    void saveCurrentSettingRecord();
    void setCurrentRecordName(const QString &label);
    QString getCurrentRecordName();
    void clearContent();

    void showEvent(QShowEvent *event);
private slots:
    void onSingleRangChanged(bool isSingle);

    void on_pushButton_removeCol_clicked();

    void on_pushButton_addCol_clicked();

    void on_pushButton_clear_clicked();

    void on_pushButton_generate_clicked();

    void on_tableWidget_device_itemChanged(QTableWidgetItem *item);

private:

    void init();

    void adjustColNo();

    bool judgeSave();

//    void addColumn(int colNo, double tempValue = 0.0);

    Ui::FormDeviceContent *ui;


};

#endif // FORMDEVICECONTENT_H
