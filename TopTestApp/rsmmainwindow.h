#ifndef RSMMAINWINDOW_H
#define RSMMAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class RSMMainWindow;
}

class RSMMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit RSMMainWindow(QWidget *parent = 0);
    ~RSMMainWindow();

    ///load system global setting
    ///and apply it
    void loadSystemSettings();

public slots:
    void onStartShowDialog();
    void onEndShowDialog();

protected:
    void closeEvent(QCloseEvent *event);

private:
    Ui::RSMMainWindow *ui;
    QWidget* mockWidget;
    int showMockCount;
};

#endif // RSMMAINWINDOW_H
