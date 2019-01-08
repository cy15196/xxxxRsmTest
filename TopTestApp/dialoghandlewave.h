#ifndef DIALOGHANDLEWAVE_H
#define DIALOGHANDLEWAVE_H

#include <QWidget>
#include "dialogbase.h"

namespace Ui {
class DialogHandleWave;
}

namespace TopTest {
    struct WaveInfo;
}

class DialogHandleWave : public DialogBase
{
    Q_OBJECT

public:
    explicit DialogHandleWave(QWidget *parent = nullptr);
    ~DialogHandleWave();

    void setWaveInfo(const TopTest::WaveInfo& waveInfo);

    void onButtonClicked(int role);

signals:
    void waveInfoChanged(const TopTest::WaveInfo& waveInfo);

private:
    Ui::DialogHandleWave *ui;
};

#endif // DIALOGHANDLEWAVE_H
