#ifndef FORMWELCOME_H
#define FORMWELCOME_H

#include <QWidget>
#include "toptestwidgets_global.h"

namespace Ui {
class FormWelcome;
}

class QListWidgetItem;

class TOPTESTWIDGETSSHARED_EXPORT FormWelcome : public QWidget
{
    Q_OBJECT

public:
    explicit FormWelcome(QWidget *parent = 0);
    ~FormWelcome();

signals:
    void needToOpenFile(const QString&fileName);

private slots:
    void on_listWidget_history_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);
    void refreshHistoryList();

    void on_listWidget_history_itemDoubleClicked(QListWidgetItem *item);

private:

    void init();

    void showCurrentFileInfo(const QString& filePath);

    Ui::FormWelcome *ui;
};

#endif // FORMWELCOME_H
