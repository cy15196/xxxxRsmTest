#ifndef TOPTESTMAINWINDOW_H
#define TOPTESTMAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class TopTestMainWindow;
}

class TopTestMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit TopTestMainWindow(QWidget *parent = 0);
    ~TopTestMainWindow();
    
private slots:
    void onProjectOpened(const QString& filePath);

private:

    void showWorkAreaFullScreen();

    void initToolBar();

    Ui::TopTestMainWindow *ui;
};

#endif // TOPTESTMAINWINDOW_H
