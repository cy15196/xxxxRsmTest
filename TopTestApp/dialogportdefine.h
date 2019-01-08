#ifndef DIALOGPORTDEFINE_H
#define DIALOGPORTDEFINE_H

#include <QDialog>
#include <QMap>
#include <QPair>

#include "dialogbase.h"

namespace Ui {
class DialogPortDefine;
}

class DialogPortDefine : public DialogBase
{
    Q_OBJECT

public:
    explicit DialogPortDefine(QWidget *parent = 0);
    ~DialogPortDefine();

    void setCurrentPortInfo(const QMap<int,QPair<QString,QColor>> & portInfo);
    QMap<int,QPair<QString,QColor>>  getCurrentPortInfo();
signals:
    void portInfoChanged(const QMap<int,QPair<QString,QColor>> & portInfo);

private slots:
    void onButtonClicked(int role);

private:
    Ui::DialogPortDefine *ui;

    QMap<int,QPair<QString,QColor>> oldPortInfo;
};

#endif // DIALOGPORTDEFINE_H
