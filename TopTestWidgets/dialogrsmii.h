#ifndef DIALOGRSMII_H
#define DIALOGRSMII_H

#include <QDialog>

namespace Ui {
class DialogRSMII;
}

class DialogRSMII : public QDialog
{
    Q_OBJECT

public:
    explicit DialogRSMII(QWidget *parent = 0);
    ~DialogRSMII();

private:
    Ui::DialogRSMII *ui;
};

#endif // DIALOGRSMII_H
