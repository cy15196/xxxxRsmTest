#ifndef FORMPROCESSCONTENT_H
#define FORMPROCESSCONTENT_H

#include <QWidget>

namespace Ui {
class FormProcessContent;
}

class FormProcessContent : public QWidget
{
    Q_OBJECT

public:
    explicit FormProcessContent(QWidget *parent = 0);
    ~FormProcessContent();

private:
    Ui::FormProcessContent *ui;
};

#endif // FORMPROCESSCONTENT_H
