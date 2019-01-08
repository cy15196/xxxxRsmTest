#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QPushButton;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void doRtuQuery();
    void doTcpQuery();
private:
    QPushButton* pbRtu;
    QPushButton* pbTcp;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
