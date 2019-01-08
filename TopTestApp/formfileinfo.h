#ifndef FORMFILEINFO_H
#define FORMFILEINFO_H

#include <QWidget>
#include "dialogbase.h"

namespace Ui {
class FormFileInfo;
}

class FormFileInfo : public DialogBase
{
    Q_OBJECT

public:
    explicit FormFileInfo(QWidget *parent = 0);
    ~FormFileInfo();

    void onButtonClicked(int role);
    int standerButtons();

    void fillCurrentFileInfo(const QVariantMap& fileInfo);

signals:
    void fileInfoChanged(const QVariantMap& fileInfo);

private:

    void init();

    QVariantMap getFileInfoFromWidget();

    Ui::FormFileInfo *ui;
};

#endif // FORMFILEINFO_H
