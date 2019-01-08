#ifndef FORMFILESETTINGBASEWIDGET_H
#define FORMFILESETTINGBASEWIDGET_H

#include <QWidget>

namespace Ui {
class FormFileSettingBaseWidget;
}

class QListWidgetItem;

class FileSettingWidget: public QWidget
{
    Q_OBJECT
public:
    FileSettingWidget(QWidget* parent = 0);
    virtual QString getFileTypeDisplayText() = 0;
    virtual QString getFileTypeSettingKeyName() = 0;

    virtual void loadSettigRecord(const QVariant& recordName) = 0;
    virtual void saveCurrentSettingRecord() = 0;

    virtual void setCurrentRecordName(const QString& label) = 0;
    virtual QString getCurrentRecordName() = 0;

    virtual void clearContent() =0;

    ///setting files deal with system
    void writeDataToSettingFile(const QString& key, const QVariant& data);
    QVariant readDataFromSettingFile(const QString& key);
    void removeDataInSettingFile(const QString& key);

    virtual QMap<QString,QVariant> saveWidgetContentToMap();
    void loadWidgetContentMap(const QMap<QString,QVariant>& map);

    static void writeLastEditRecordToSettingFile(const QString& fileTypeSettingKeyName, const QString &key);
    static QString getLastEditRecordName(const QString& fileTypeSettingKeyName);
    static void updateLastEditRecordName(const QString& fileTypeSettingKeyName);

signals:
    void contentChanged();

};

class FormFileSettingBaseWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FormFileSettingBaseWidget(FileSettingWidget* centralWidget, QWidget *parent = 0);
    ~FormFileSettingBaseWidget();

    void reloadSystemSetting();

private slots:

    void on_toolButton_new_clicked();

    void on_toolButton_import_clicked();

    void on_toolButton_del_clicked();

    void on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

    void on_pushButton_save_clicked();

    void on_pushButton_reset_clicked();

    void onCurrentContentChanged();

private:
    void init();
    FileSettingWidget* centralWidget;
    Ui::FormFileSettingBaseWidget *ui;
};

#endif // FORMFILESETTINGBASEWIDGET_H
