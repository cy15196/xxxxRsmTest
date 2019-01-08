#ifndef DIALOGHISTORYLINES_H
#define DIALOGHISTORYLINES_H

#include <QDialog>

namespace Ui {
class DialogHistoryLines;
}

class DialogHistoryLines : public QDialog
{
    Q_OBJECT

public:
    explicit DialogHistoryLines(QWidget *parent = 0);
    ~DialogHistoryLines();

private:
    Ui::DialogHistoryLines *ui;
};

#endif // DIALOGHISTORYLINES_H
