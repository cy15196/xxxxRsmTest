#ifndef FORMMODULETAB_H
#define FORMMODULETAB_H

#include <QWidget>
#include <QMap>

class QToolButton;

namespace Ui {
class FormModuleTab;
}

class FormModuleTab : public QWidget
{
    Q_OBJECT

public:
    explicit FormModuleTab(QWidget *parent = 0);
    ~FormModuleTab();

    QList<QWidget*> getWidgetList();


signals:
    void changedToWidget(QWidget* w);
    void refreshWelcomeWidget();
    void needToOpenFile(const QString& filePath);

private slots:
    void onToolButtonToggled(bool checked);

    void onFileOpened();

    void onShowWidget();

private:

    void init();

    Ui::FormModuleTab *ui;
    QMap<QToolButton*, QWidget*> widgetMap;
};

#endif // FORMMODULETAB_H
