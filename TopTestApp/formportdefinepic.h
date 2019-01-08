#ifndef FORMPORTDEFINEPIC_H
#define FORMPORTDEFINEPIC_H

#include <QWidget>

namespace Ui {
class FormPortDefinePic;
}

class FormPortDefinePic : public QWidget
{
    Q_OBJECT

public:
    explicit FormPortDefinePic(QWidget *parent = 0);
    ~FormPortDefinePic();

    void clearContent();

    void setPushButtonPos(const QPoint &pos);

signals:
    void contentChanged();

private slots:
    void on_pushButton_clicked();

private:
    Ui::FormPortDefinePic *ui;

};

#endif // FORMPORTDEFINEPIC_H
