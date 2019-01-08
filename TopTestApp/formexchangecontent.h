#ifndef FORMEXCHANGECONTENT_H
#define FORMEXCHANGECONTENT_H

#include <QWidget>

namespace Ui {
class FormExchangeContent;
}

class FormExchangeContent : public QWidget
{
    Q_OBJECT

public:
    explicit FormExchangeContent(QWidget *parent = 0);
    ~FormExchangeContent();

    void loadSettingFileToSystem(const QString& fileName);
    void saveSettingToFile(const QString& fileName);

    void updateProgress(const QString& state, int progress);

signals:
    void needToReloadSystemSettings();

private slots:
    void on_toolButton_import_clicked();

    void on_toolButton_export_clicked();

private:
    Ui::FormExchangeContent *ui;
};

#endif // FORMEXCHANGECONTENT_H
