#ifndef DIALOGBASE_H
#define DIALOGBASE_H

#include <QWidget>
#include <QDialogButtonBox>
class DialogBase: public QWidget
{
    Q_OBJECT
public:
    DialogBase(QWidget*parent = 0);
    ~DialogBase();

    virtual void onButtonClicked(int button) = 0;
    virtual int standerButtons();

};

#endif // DIALOGBASE_H
