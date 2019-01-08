#ifndef RSMDIALOGBASE_H
#define RSMDIALOGBASE_H

#include <QDialog>

namespace Ui {
class RsmDialogBase;
}

class RsmDialogBase : public QDialog
{
    Q_OBJECT

public:
    explicit RsmDialogBase(QWidget *parent = 0);
    ~RsmDialogBase();

private:
    Ui::RsmDialogBase *ui;
};

#endif // RSMDIALOGBASE_H
