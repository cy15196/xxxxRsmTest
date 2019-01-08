#include "formfilesettingbasewidget.h"
#include "ui_formfilesettingbasewidget.h"

#include <QSettings>
#include <QDebug>
#include <QLineEdit>
#include <QTextEdit>
#include <QRadioButton>
#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QBitArray>
#include <QBuffer>
#include <QFileDialog>

#include "formdraglabel.h"
#include "formportdefinepic.h"
#include "rsmcoresignalenum.h"
#include "formdatatable.h"
#include "formdevicetable.h"

FileSettingWidget::FileSettingWidget(QWidget *parent)
    :QWidget(parent)
{

}

void FileSettingWidget::writeDataToSettingFile(const QString &key, const QVariant &data)
{
    //save record
    QSettings appSetting(QCoreApplication::organizationName(),QCoreApplication::applicationName());
    appSetting.beginGroup("FileSettings");
    appSetting.beginGroup(getFileTypeSettingKeyName());

    appSetting.remove(key);
    appSetting.setValue(key,data);

    appSetting.endGroup();
    appSetting.endGroup();
}

void FileSettingWidget::writeLastEditRecordToSettingFile(const QString &fileTypeSettingKeyName, const QString &key)
{
    //save record
    QSettings appSetting(QCoreApplication::organizationName(),QCoreApplication::applicationName());
    appSetting.beginGroup("LastEditSettingFile");

    appSetting.setValue(fileTypeSettingKeyName,key);

    appSetting.endGroup();
}

QString FileSettingWidget::getLastEditRecordName(const QString &fileTypeSettingKeyName)
{
    //save record
    QSettings appSetting(QCoreApplication::organizationName(),QCoreApplication::applicationName());
    appSetting.beginGroup("LastEditSettingFile");
    QString lastRecordName = appSetting.value(fileTypeSettingKeyName).toString();
    appSetting.endGroup();

    appSetting.beginGroup("FileSettings");
    appSetting.beginGroup(fileTypeSettingKeyName);

    if(lastRecordName.isEmpty() || !appSetting.childKeys().contains(lastRecordName))
    {
        if(!appSetting.childKeys().isEmpty())
        {
            lastRecordName = appSetting.childKeys().first();
            writeLastEditRecordToSettingFile(fileTypeSettingKeyName,lastRecordName);
        }
    }

    appSetting.endGroup();
    appSetting.endGroup();

    return lastRecordName;
}

void FileSettingWidget::updateLastEditRecordName(const QString &fileTypeSettingKeyName)
{
    getLastEditRecordName(fileTypeSettingKeyName);
}

QVariant FileSettingWidget::readDataFromSettingFile(const QString &key)
{
    QSettings appSetting(QCoreApplication::organizationName(),QCoreApplication::applicationName());
    appSetting.beginGroup("FileSettings");
    appSetting.beginGroup(getFileTypeSettingKeyName());

    QVariant value = appSetting.value(key);

    appSetting.endGroup();
    appSetting.endGroup();

    return value;
}

void FileSettingWidget::removeDataInSettingFile(const QString &key)
{
    QCoreApplication*a = QCoreApplication::instance();
    QSettings appSetting(a->organizationName(),a->applicationName());
    appSetting.beginGroup("FileSettings");
    appSetting.beginGroup(getFileTypeSettingKeyName());

    appSetting.remove(key);

    appSetting.endGroup();
    appSetting.endGroup();
}

QMap<QString, QVariant> FileSettingWidget::saveWidgetContentToMap()
{
    QMap<QString, QVariant> ret;

    foreach (QWidget* childWidget, this->findChildren<QWidget*>()) {
        QLineEdit* lineEdit = qobject_cast<QLineEdit*>(childWidget);
        if(lineEdit)
        {
            ret.insert(childWidget->objectName(),lineEdit->text());
            continue;
        }
        QLabel* lable = qobject_cast<QLabel*>(childWidget);
        if(lable)
        {
            if(!lable->text().isEmpty())
                ret.insert(childWidget->objectName(),lable->text());
            else if(lable->pixmap())
            {
                QByteArray ba;
                QBuffer buffer(&ba);
                buffer.open(QIODevice::WriteOnly);
                QImage image = lable->pixmap()->toImage();
                image.save(&buffer, "PNG");
                ret.insert(childWidget->objectName(),ba);
            }
            continue;
        }

        QTextEdit* textEdit = qobject_cast<QTextEdit*>(childWidget);
        if(textEdit)
        {
            ret.insert(childWidget->objectName(),textEdit->toHtml());
            continue;
        }
        QRadioButton* radioEdit = qobject_cast<QRadioButton*>(childWidget);
        if(radioEdit)
        {
            ret.insert(childWidget->objectName(),radioEdit->isChecked());
            continue;
        }

        QSpinBox* spinEdit = qobject_cast<QSpinBox*>(childWidget);
        if(spinEdit)
        {
            ret.insert(childWidget->objectName(),spinEdit->value());
            continue;
        }

        QDoubleSpinBox* doubleSpinEdit = qobject_cast<QDoubleSpinBox*>(childWidget);
        if(doubleSpinEdit)
        {
            ret.insert(childWidget->objectName(),doubleSpinEdit->value());
            continue;
        }

        QComboBox* comboBoxEdit = qobject_cast<QComboBox*>(childWidget);
        if(comboBoxEdit)
        {
            ret.insert(childWidget->objectName(),comboBoxEdit->currentText());
            continue;
        }

        FormDragLabel* dragEdit = qobject_cast<FormDragLabel*>(childWidget);
        if(dragEdit)
        {
            ret.insert(childWidget->objectName(),dragEdit->getPos());
            continue;
        }

        FormDataTable* dataTable = qobject_cast<FormDataTable*>(childWidget);
        if(dataTable)
        {
            ret.insert(childWidget->objectName(),dataTable->saveTableDataToMap());
            continue;
        }

        FormDeviceTable* deviceTable = qobject_cast<FormDeviceTable*>(childWidget);
        if(deviceTable)
        {
            ret.insert(childWidget->objectName(),deviceTable->saveTableToMap());
            continue;
        }

//        FormPortDefinePic* picEdit = qobject_cast<FormPortDefinePic*>(childWidget);
//        if(picEdit)
//        {
//            ret.insert(childWidget->objectName(),picEdit->);
//            continue;
//        }

    }

    return ret;
}

void FileSettingWidget::loadWidgetContentMap(const QMap<QString, QVariant> &map)
{
    foreach (QString objName, map.keys()) {
        QWidget* childWidget = this->findChild<QWidget*>(objName);
        if(!childWidget)
            continue;

        QLineEdit* lineEdit = qobject_cast<QLineEdit*>(childWidget);
        if(lineEdit)
        {
            lineEdit->setText(map.value(objName).toString());
            continue;
        }

        QTextEdit* textEdit = qobject_cast<QTextEdit*>(childWidget);
        if(textEdit)
        {
            textEdit->setHtml(map.value(objName).toString());
            continue;
        }

        QRadioButton* radioEdit = qobject_cast<QRadioButton*>(childWidget);
        if(radioEdit)
        {
            radioEdit->setChecked(map.value(objName).toBool());
            continue;
        }

        QSpinBox* spinEdit = qobject_cast<QSpinBox*>(childWidget);
        if(spinEdit)
        {
            spinEdit->setValue(map.value(objName).toInt());
            continue;
        }

        QDoubleSpinBox* doubleSpinEdit = qobject_cast<QDoubleSpinBox*>(childWidget);
        if(doubleSpinEdit)
        {
            doubleSpinEdit->setValue(map.value(objName).toDouble());
            continue;
        }

        QComboBox* comboBoxEdit = qobject_cast<QComboBox*>(childWidget);
        if(comboBoxEdit)
        {
            comboBoxEdit->setCurrentText(map.value(objName).toString());
            continue;
        }

        FormDragLabel* dragEdit = qobject_cast<FormDragLabel*>(childWidget);
        if(dragEdit)
        {
            dragEdit->move(map.value(objName).toPoint());
            continue;
        }

        FormDataTable* dataTable = qobject_cast<FormDataTable*>(childWidget);
        if(dataTable)
        {
            dataTable->loadMapToTable(map.value(objName).toMap());
            continue;
        }

        FormDeviceTable* deviceTable = qobject_cast<FormDeviceTable*>(childWidget);
        if(deviceTable)
        {
            deviceTable->loadDataToTable(map.value(objName).toMap());
            continue;
        }


        QLabel* labelEdit = qobject_cast<QLabel*>(childWidget);
        if(labelEdit)
        {
            QVariant value = map.value(objName);
            QByteArray ba = value.toByteArray();
            QImage im;
            if(im.loadFromData(ba,"PNG"))
            {
                labelEdit->setPixmap(QPixmap::fromImage(im));
            }
            else
            {
                labelEdit->setText(value.toString());
            }
        }

    }
}




FormFileSettingBaseWidget::FormFileSettingBaseWidget(FileSettingWidget *centralWidget, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormFileSettingBaseWidget)
{
    ui->setupUi(this);
    this->centralWidget = centralWidget;
    ui->toolButton_import->setVisible(false);
    init();
}

FormFileSettingBaseWidget::~FormFileSettingBaseWidget()
{
    delete ui;
}


void FormFileSettingBaseWidget::init()
{

    ui->gridLayout_centralWidget->addWidget(centralWidget);
    centralWidget->setEnabled(false);
    ui->label_fileType->setText(centralWidget->getFileTypeDisplayText());


    QCoreApplication*a = QApplication::instance();
    QSettings appSetting(a->organizationName(),a->applicationName());
    appSetting.beginGroup("FileSettings");
    appSetting.beginGroup(centralWidget->getFileTypeSettingKeyName());
    QStringList recordKeyList = appSetting.allKeys();
    appSetting.endGroup();
    appSetting.endGroup();

    foreach (QString record, recordKeyList) {
        QListWidgetItem* item = new QListWidgetItem(record);
        ui->listWidget->addItem(item);
    }
    ui->listWidget->setCurrentRow(0);

    connect(centralWidget,SIGNAL(contentChanged()),
            this,SLOT(onCurrentContentChanged()));

//    ui->stackedWidget_content->addWidget(
//                new FormProductContent(this));
//    ui->stackedWidget_content->addWidget(
//                new FormDeviceContent(this));
//    ui->stackedWidget_content->addWidget(
//                new FormProgressHandle(this));
}

void FormFileSettingBaseWidget::on_toolButton_new_clicked()
{
    int i = ui->listWidget->count();
    QString label = QString("new Record %1").arg(++i);
    QListWidgetItem* item = new QListWidgetItem(label);
    ui->listWidget->addItem(item);
    ui->listWidget->setCurrentItem(item);

    item->setText(item->text()+"*");
}

void FormFileSettingBaseWidget::on_toolButton_import_clicked()
{
    QStringList fileList = QFileDialog::getOpenFileNames(this,tr("Load setting file content from file"),
                                  "D:/","*.rsm");

    foreach (QString file, fileList) {

    }

}

void FormFileSettingBaseWidget::on_toolButton_del_clicked()
{

    QListWidgetItem* item = ui->listWidget->takeItem(ui->listWidget->currentRow());
    if(item)
    {
        centralWidget->removeDataInSettingFile(item->text());
        centralWidget->updateLastEditRecordName(centralWidget->getFileTypeSettingKeyName());
        delete item;
    }
}

void FormFileSettingBaseWidget::on_listWidget_currentItemChanged(
        QListWidgetItem *current, QListWidgetItem *previous)
{
    centralWidget->blockSignals(true);
    centralWidget->clearContent();
    if(current)
    {
        centralWidget->setEnabled(true);
        QString key = current->text();
        centralWidget->setCurrentRecordName(key);
        QMap<QString,QVariant> widgetInfoMap = current->data(Qt::UserRole).toMap();
        if(widgetInfoMap.isEmpty())
        {
            centralWidget->loadSettigRecord(key);
        }
        else
        {
            centralWidget->loadWidgetContentMap(widgetInfoMap);
        }
    }
    else
        centralWidget->setEnabled(false);
    centralWidget->blockSignals(false);
}

void FormFileSettingBaseWidget::on_pushButton_save_clicked()
{
    QListWidgetItem* item = ui->listWidget->currentItem();
    if(item)
    {
        if(item->text().endsWith("*"))
        {
            QString text = item->text();
            text.chop(1);
            centralWidget->removeDataInSettingFile(text);
            item->setText(centralWidget->getCurrentRecordName());
            centralWidget->saveCurrentSettingRecord();

            centralWidget->writeLastEditRecordToSettingFile(
                        centralWidget->getFileTypeSettingKeyName(),
                        text);
        }
    }
}

void FormFileSettingBaseWidget::on_pushButton_reset_clicked()
{
    centralWidget->blockSignals(true);
    QListWidgetItem* item = ui->listWidget->currentItem();
    if(item)
    {
        QString currentKey = item->text();
        if(currentKey.endsWith("*"))
            currentKey.chop(1);
        centralWidget->clearContent();
        centralWidget->loadSettigRecord(currentKey);
    }
    centralWidget->blockSignals(false);
}

void FormFileSettingBaseWidget::onCurrentContentChanged()
{
    QListWidgetItem* item = ui->listWidget->currentItem();
    if(item)
    {
        item->setData(Qt::UserRole,centralWidget->saveWidgetContentToMap());
        if(!item->text().endsWith("*"))
        {
            item->setText(item->text()+"*");
        }
    }
}

void FormFileSettingBaseWidget::reloadSystemSetting()
{

    on_listWidget_currentItemChanged(0,0);


    QCoreApplication*a = QCoreApplication::instance();
    QSettings appSetting(a->organizationName(),a->applicationName());
    appSetting.beginGroup("FileSettings");
    appSetting.beginGroup(centralWidget->getFileTypeSettingKeyName());
    QStringList recordKeyList = appSetting.allKeys();
    appSetting.endGroup();
    appSetting.endGroup();

    foreach (QString record, recordKeyList) {
        QListWidgetItem* item = new QListWidgetItem(record);
        ui->listWidget->addItem(item);
    }
    ui->listWidget->setCurrentRow(0);

}

