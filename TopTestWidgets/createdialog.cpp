#include "createdialog.h"
#include "ui_createdialog.h"
#include <QFileDialog>
#include <QPushButton>
#include <QSettings>
#include <QDebug>
#include <QFile>
#include <QDir>

#include "formcreatedataanalysis.h"

extern Q_CORE_EXPORT int qt_ntfs_permission_lookup;

bool checkPathAviable(const QString& path)
{
    qt_ntfs_permission_lookup++;
    QFile pathFile(path);
    if(! (pathFile.permissions() &  QFile::WriteUser))
        return false;
    qDebug()<<pathFile.permissions();
    QFile testFile(path+"/test.text");
    qt_ntfs_permission_lookup--;
    return testFile.open(QFile::WriteOnly);

}

CreateDialog::CreateDialog(TopTest::ProjectInfo *info, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateDialog),
    info(info)
{
    ui->setupUi(this);
    setWindowModality(Qt::ApplicationModal);

    addPage(new FormCreateDataAnalysis(QMap<QString,QVariant>(), this));

    ui->pathEdit->setText(qApp->applicationDirPath());
    ui->nameEdit->setText("Unnamed");

    ///read setting from file
    QSettings appSetting(QApplication::organizationName(),QApplication::applicationName());
    QString defaultPath = appSetting.value("DefalutPath","").toString();
    ui->asDefault->setChecked(false);
    if(QDir(defaultPath).exists())
        ui->pathEdit->setText(defaultPath);
    qDebug()<<"Read settings to file "<<appSetting.value("SaveDefalutPath",false).toBool()<<" "<<appSetting.value("DefalutPath","").toString();
}

CreateDialog::~CreateDialog()
{
    QSettings appSetting(QApplication::organizationName(), QApplication::applicationName());
    if(ui->asDefault->isChecked())
        appSetting.setValue("DefalutPath",ui->pathEdit->text());
    delete ui;
}

void CreateDialog::addPage(ProjectSetting *page)
{
    ui->tabWidget->addTab(page, page->windowTitle());
    if (ui->tabWidget->count() == 1)
        on_tabWidget_currentChanged(0);
}

void CreateDialog::accept()
{
    info->isValid = true;
    ProjectSetting* page = qobject_cast<ProjectSetting*> (ui->tabWidget->currentWidget());
    info->projectMap = page->getDataMap();
    return QDialog::accept();
}

void CreateDialog::on_nameEdit_textChanged(const QString &/*arg1*/)
{
    if (censorPath() && censorName())
        ok(true);
    else
        ok(false);
}

void CreateDialog::on_pathEdit_textChanged(const QString &/*arg1*/)
{
    if (censorPath() && censorName())
        ok(true);
    else
        ok(false);
}

void CreateDialog::on_searchButton_clicked()
{
    QString p = QFileDialog::getExistingDirectory(this, tr("Select Directory"), ui->pathEdit->text());
    if (p.isEmpty())
        return;
    ui->pathEdit->setText(p);
}

void CreateDialog::on_tabWidget_currentChanged(int index)
{
//    for (int i=0; i<ui->tabWidget->count(); ++i)
//        dynamic_cast<ProjectSetting *>(ui->tabWidget->widget(i))->setSelected(false);
//    dynamic_cast<ProjectSetting *>(ui->tabWidget->widget(index))->setSelected(true);
    info->type = (TopTest::ProjectType)index;
//    info->projectType.string = ui->tabWidget->widget(index)->windowTitle();
}

void CreateDialog::ok(bool isOk)
{
    if (isOk)
    {
        QString n = ui->nameEdit->text();
        QString p = ui->pathEdit->text();
        if (!p.endsWith("/"))
            p += "/";
//        emit pathChanged(p, n);
        info->name = n;
        info->path = p;
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    }
    else
    {
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    }
}

bool CreateDialog::censorName()
{
    QString n = ui->nameEdit->text();
    QString p = ui->pathEdit->text();
    if (!p.endsWith("/"))
        p += "/";
    p += n;
    if (QFile::exists(p))
    {
        ui->nameEdit->setStyleSheet("color: rgb(255, 0, 0);");
        ui->nameInfo->setText("<html><head/><body><p><span style=\" color:#ff0000;\">file is exists</span></p></body></html>");
        return false;
    }
    else
    {
        ui->nameEdit->setStyleSheet("color: rgb(0, 0, 0);");
        ui->nameInfo->setText("");
        return true;
    }
}

bool CreateDialog::censorPath()
{
    QString p = ui->pathEdit->text();
    if (QFile::exists(p))
    {
        if(!checkPathAviable(p))
        {
            ui->pathEdit->setStyleSheet("color: rgb(255, 0, 0);");
            ui->pathInfo->setText("<html><head/><body><p><span style=\" color:#ff0000;\">has no right to create file on such path</span></p></body></html>");
            return false;
        }
        ui->pathEdit->setStyleSheet("color: rgb(0, 0, 0);");
        ui->pathInfo->setText("");
        return true;
    }
    else
    {
        ui->pathEdit->setStyleSheet("color: rgb(255, 0, 0);");
        ui->pathInfo->setText("<html><head/><body><p><span style=\" color:#ff0000;\">path is not exists</span></p></body></html>");
        return false;
    }
}
