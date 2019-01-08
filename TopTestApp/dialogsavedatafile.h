#ifndef DIALOGSAVEDATAFILE_H
#define DIALOGSAVEDATAFILE_H

#include <QDialog>

#include "dialogbase.h"

namespace Ui {
class DialogSaveDataFile;
}

namespace TopTest {
struct  RecordInfoData;
}

class QAbstractButton;

class DialogSaveDataFile : public DialogBase
{
    Q_OBJECT

public:
    explicit DialogSaveDataFile(QWidget *parent = 0);
    ~DialogSaveDataFile();

    void setCurrentProductName(const QString& path);
    void setCurrentFileName(const QString& fileName);
    void setFileInfo(TopTest::RecordInfoData* infoData);

signals:
    void saveCurrentFileToPath(const QString& path);

private slots:
    void onButtonClicked(int role);

    void on_pushButton_changeDir_clicked();

    void onFileNameChanged();

private:

    QString saveFilePath;
    TopTest::RecordInfoData* infoData = nullptr;

    Ui::DialogSaveDataFile *ui;
};

#endif // DIALOGSAVEDATAFILE_H
