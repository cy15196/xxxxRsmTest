#ifndef DIALOGPROGRESSHANDLE_H
#define DIALOGPROGRESSHANDLE_H

#include "formfilesettingbasewidget.h"

namespace Ui {
class DialogProgressHandle;
}

class QTableWidgetItem;
class FormHeaderTable;
class FormDataTable;
class QListWidget;


class  FormProgressHandle : public FileSettingWidget
{
    Q_OBJECT

public:
    explicit FormProgressHandle(QWidget *parent = 0);
    ~FormProgressHandle();

    // FileSettingWidget interface
    QString getFileTypeDisplayText();
    QString getFileTypeSettingKeyName();
    void loadSettigRecord(const QVariant&  recordName);
    void saveCurrentSettingRecord();
    void setCurrentRecordName(const QString &label);
    QString getCurrentRecordName();
    void clearContent();
    QMap<QString,QVariant> saveWidgetContentToMap();

protected:
    void changeEvent(QEvent* event);

private slots:
    void on_changedToType(int type);

    void onTableDataEdited(int type);

    void onDataSaved();

    void on_tableWidget_wave_itemChanged(QTableWidgetItem *item);

    void on_tableWidget_wave_itemSelectionChanged();

    void changeToRowWave(int rowNo);

private:

    void setRowTemperatureEditable(int rowNo,bool editable);

//    void initPortChoose();
    void init();

    Ui::DialogProgressHandle *ui;

    FormHeaderTable *current_HeaderTable = 0;
    FormDataTable *current_DataTable = 0;
    QListWidget *current_ProgressType = 0;
};

#endif // DIALOGPROGRESSHANDLE_H
