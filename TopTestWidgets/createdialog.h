#ifndef CREATEDIALOG_H
#define CREATEDIALOG_H

#include <QDialog>
#include "toptestwidgets_global.h"
#include "topteststructdata.h"

namespace Ui {
class CreateDialog;
}

class ProjectSetting;

class TOPTESTWIDGETSSHARED_EXPORT CreateDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateDialog(TopTest::ProjectInfo *info, QWidget *parent = 0);
    ~CreateDialog();

    void addPage(ProjectSetting *page);

public slots:
    void accept();

private slots:
    void on_nameEdit_textChanged(const QString &arg1);
    void on_pathEdit_textChanged(const QString &arg1);
    void on_searchButton_clicked();
    void on_tabWidget_currentChanged(int index);

private:
    void ok(bool isOk);
    bool censorName();
    bool censorPath();

    Ui::CreateDialog *ui;
    TopTest::ProjectInfo *info = nullptr;
};

#endif // CREATEDIALOG_H
