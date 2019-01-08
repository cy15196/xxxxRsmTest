#ifndef PORTMANAGERMAINWINDOW_H
#define PORTMANAGERMAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class PortManagerMainWindow;
}

class PortManagerMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PortManagerMainWindow(QWidget *parent = nullptr);
    ~PortManagerMainWindow();

    bool nativeEvent(const QByteArray &eventType, void *message, long *result);
private:
    Ui::PortManagerMainWindow *ui;
};

#endif // PORTMANAGERMAINWINDOW_H
