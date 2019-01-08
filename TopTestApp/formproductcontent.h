#ifndef FORMPRODUCTCONTENT_H
#define FORMPRODUCTCONTENT_H

#include "formfilesettingbasewidget.h"

namespace Ui {
class FormProductContent;
}

class FormProductContent : public FileSettingWidget
{
    Q_OBJECT

public:
    explicit FormProductContent(QWidget *parent = 0);
    ~FormProductContent();

    // FileSettingWidget interface
public:
    QString getFileTypeDisplayText();
    QString getFileTypeSettingKeyName();
    void loadSettigRecord(const QVariant&  recordName);
    void saveCurrentSettingRecord();
    void setCurrentRecordName(const QString &label);
    QString getCurrentRecordName();
    void clearContent();

//private slots:
//    void on_pushButton_clicked();

private:
    Ui::FormProductContent *ui;
};

#endif // FORMPRODUCTCONTENT_H
